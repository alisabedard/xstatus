// Copyright 2017, Jeffrey E. Bedard
#include "battery.h"
#include "XSWidget.h"
#include "config.h"
#include "font.h"
#include "libjb/log.h"
#include "libjb/macros.h"
#include "libjb/util.h"
#include "util.h"
#include "xstatus.h"
//#define XSTATUS_BATTERY_TEST
// get percent value, maximum 100, -1 on error
static int8_t get_percent(void)
{
	return JB_MIN(xstatus_system_value(XSTATUS_SYSFILE_BATTERY), 100);
}
// index into gc array, keeps gc array private
enum BATGCs { BATTERY_GC_BACKGROUND, BATTERY_GC_AC, BATTERY_GC_BATTERY,
	BATTERY_GC_CRITICAL, BATTERY_GC_SIZE };
// Selects a gc to use based on ac/battery status
static uint8_t get_gc(const uint8_t pct)
{
	return xstatus_system_value(XSTATUS_SYSFILE_AC) ? BATTERY_GC_AC : pct
		< XSTATUS_CONST_CRITICAL_PERCENT
		? BATTERY_GC_CRITICAL : BATTERY_GC_BATTERY;
}
static uint8_t format(char * buf, const uint8_t sz, const uint8_t pct)
{
	return snprintf(buf, sz, " %d%% ", pct);
}
static void draw_percent(struct XSWidget * widget, const uint8_t pct)
{
	enum {BUF_SZ = 7};
	char buf[BUF_SZ];
	xcb_image_text_8(widget->connection, format(buf, BUF_SZ, pct),
		widget->window, widget->foreground, widget->x,
		xstatus_get_font_size().h, buf);
}
static void set_gc(xcb_connection_t * restrict xc, const xcb_window_t w,
	xcb_gcontext_t * restrict gc, const char * restrict fg)
{
	xstatus_create_gc(xc, *gc = xcb_generate_id(xc), w, fg,
		XSTATUS_BATTERY_BACKGROUND_COLOR);
}
static void initialize_gcs(xcb_connection_t * restrict xc,
	const xcb_window_t w, xcb_gcontext_t * restrict gc)
{
#define SETGC(color) set_gc(xc, w, gc + BATTERY_GC_##color, \
	XSTATUS_BATTERY_##color##_COLOR);
	SETGC(BACKGROUND); SETGC(AC); SETGC(BATTERY); SETGC(CRITICAL);
#undef SETGC
}
static xcb_rectangle_t get_rectangle(const struct JBDim range)
{
	return (xcb_rectangle_t){.x=range.start,
		.y = (XSTATUS_CONST_HEIGHT >> 2) + 1,
		.height = XSTATUS_CONST_HEIGHT >> 1,
		.width = range.end - range.start - XSTATUS_CONST_PAD};
}
__attribute__((const))
static uint16_t get_width_for_percent(const uint16_t width,
	const uint8_t pct)
{
	return width * pct / 100;
}
static void draw_rectangles(struct XSWidget * widget, const struct JBDim
	range, const uint8_t pct)
{
	xcb_rectangle_t rect = get_rectangle(range);
	const xcb_window_t w = widget->window;
	xcb_connection_t * xc = widget->connection;
	// clear:
	xcb_poly_fill_rectangle(xc, w, widget->background, 1, &rect);
	rect.width = get_width_for_percent(rect.width, pct);
	// fill rectangle per percent full:
	xcb_poly_fill_rectangle(xc, w, widget->foreground, 1, &rect);
}
__attribute__((const))
static uint16_t get_x(const struct JBDim range)
{
	return range.start + (range.end - range.start) / 2;
}
static xcb_gcontext_t * get_gcs(xcb_connection_t * restrict xc)
{
	static xcb_gcontext_t gc[BATTERY_GC_SIZE];
	if (!*gc)
		initialize_gcs(xc, xstatus_get_window(xc), gc);
	return gc;
}
void xstatus_draw_battery(xcb_connection_t * xc, const uint16_t start,
	const uint16_t end)
{
	const int8_t pct = get_percent();
	if (pct >= 0) {
		const struct JBDim range = {.start = start, .end = end};
		xcb_gcontext_t * gcs = get_gcs(xc);
		struct XSWidget widget = {xc, xstatus_get_window(xc),
			gcs[get_gc(pct)], gcs[BATTERY_GC_BACKGROUND],
			.x = get_x(range)};
		draw_rectangles(&widget, range, pct);
		draw_percent(&widget, pct);
		xcb_flush(xc);
	}
}

// Copyright 2016, Jeffrey E. Bedard
#include "battery.h"
#include "config.h"
#include "font.h"
#include "libjb/log.h"
#include "libjb/util.h"
#include "util.h"
#include "xstatus.h"
//#define TEST
// get percent value, maximum 100
static uint8_t get_percent(void)
{
#ifdef TEST
	static int8_t p = 100;
	p -= 10;
	if (p < 0)
		return p = 100;
	return p;
#else//!TEST
	const uint8_t pct = xstatus_system_value(XSTATUS_SYSFILE_BATTERY);
	LOG("Percent: %d\n", pct);
	return JB_MIN(pct, 100);
#endif//TEST
}
// index into gc array, keeps gc array private
enum BATGCs { BATTERY_GC_BACKGROUND, BATTERY_GC_AC, BATTERY_GC_BATTERY,
	BATTERY_GC_CRITICAL, BATTERY_GC_SIZE };
// Selects a gc to use based on ac/battery status
static enum BATGCs get_gc(const uint8_t pct)
{
	return xstatus_system_value(XSTATUS_SYSFILE_AC) ? BATTERY_GC_AC : pct
		< XSTATUS_CONST_CRITICAL_PERCENT
		? BATTERY_GC_CRITICAL : BATTERY_GC_BATTERY;
}
static void draw_percent(xcb_connection_t * restrict xc, const xcb_gc_t gc,
	const uint8_t pct, const int16_t x)
{
	const uint8_t buf_sz = 7;
	char buf[buf_sz];
	const uint8_t l = snprintf(buf, buf_sz, " %d%% ", pct);
	xcb_image_text_8(xc, l, xstatus_get_window(xc),
		gc, x, xstatus_get_font_size().h, buf);
}
static void set_gc(xcb_connection_t * restrict xc, const xcb_window_t w,
	xcb_gc_t * restrict gc, const char * restrict fg)
{
	xstatus_create_gc(xc, *gc = xcb_generate_id(xc), w, fg,
		XSTATUS_BATTERY_BACKGROUND_COLOR);
}
static void init_gcs(xcb_connection_t * restrict xc, const xcb_window_t w,
	xcb_gc_t * restrict gc)
{
	set_gc(xc, w, gc + BATTERY_GC_BACKGROUND,
		XSTATUS_BATTERY_BACKGROUND_COLOR);
	set_gc(xc, w, gc + BATTERY_GC_AC,
		XSTATUS_BATTERY_AC_COLOR);
	set_gc(xc, w, gc + BATTERY_GC_BATTERY,
		XSTATUS_BATTERY_BATTERY_COLOR);
	set_gc(xc, w, gc + BATTERY_GC_CRITICAL,
		XSTATUS_BATTERY_CRITICAL_COLOR);
}
void xstatus_draw_battery(xcb_connection_t * xc, const uint16_t start,
	const uint16_t end)
{
	static xcb_gc_t gc[BATTERY_GC_SIZE];
	const xcb_window_t w = xstatus_get_window(xc);
	if (!*gc)
		init_gcs(xc, w, gc);
	const uint8_t pct = get_percent();
	const enum BATGCs a = get_gc(pct);
	xcb_rectangle_t g = {.x=start, .y = XSTATUS_CONST_HEIGHT >> 2,
		.height = XSTATUS_CONST_HEIGHT >> 1, .width = end - start
			- XSTATUS_CONST_PAD};
	++g.y;
	xcb_poly_fill_rectangle(xc, w, gc[BATTERY_GC_BACKGROUND], 1, &g);
	g.width = g.width * pct / 100;
	xcb_poly_fill_rectangle(xc, w, gc[a], 1, &g);
	draw_percent(xc, gc[a], pct, start + (end-start)/2);
	xcb_flush(xc);
}

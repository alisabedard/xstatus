// Copyright 2016, Jeffrey E. Bedard
#include "battery.h"
#include "config.h"
#include "font.h"
#include "libjb/log.h"
#include "libjb/util.h"
#include "util.h"
#include "xstatus.h"
//#define XSTATUS_DEBUG_BATTERY
#ifndef XSTATUS_DEBUG_BATTERY
#undef LOG
#define LOG(...)
#endif//!XSTATUS_DEBUG_BATTERY
//#define XSTATUS_BATTERY_TEST
// get percent value, maximum 100, -1 on error
static int8_t get_percent(void)
{
#ifdef XSTATUS_BATTERY_TEST
	static int8_t p = 100;
	p -= 10;
	if (p < 0)
		return p = 100;
	return p;
#else//!XSTATUS_BATTERY_TEST
	// Use int16_t here to prevent overflow if on AC
	const int16_t pct = xstatus_system_value(XSTATUS_SYSFILE_BATTERY);
	LOG("Percent: %d\n", pct);
	return JB_MIN(pct, 100);
#endif//XSTATUS_BATTERY_TEST
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
static void draw_percent(xcb_connection_t * restrict xc, const xcb_gc_t gc,
	const uint8_t pct, const int16_t x)
{
	enum {BUF_SZ = 7};
	char buf[BUF_SZ];
	xcb_image_text_8(xc, format(buf, BUF_SZ, pct), xstatus_get_window(xc),
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
static void set_rectangle(xcb_rectangle_t * restrict rect,
	const uint16_t start, const uint16_t end)
{
	*rect = (xcb_rectangle_t){.x=start,
		.y = (XSTATUS_CONST_HEIGHT >> 2) + 1,
		.height = XSTATUS_CONST_HEIGHT >> 1,
		.width = end - start - XSTATUS_CONST_PAD};
}
static void draw_percent_full(xcb_connection_t * restrict xc,
	const xcb_window_t w, const xcb_gc_t gc,
	xcb_rectangle_t rect, const uint8_t pct)
{
	rect.width = rect.width * pct / 100;
	// draw percent full:
	xcb_poly_fill_rectangle(xc, w, gc, 1, &rect);
}
static void draw_rectangles(xcb_connection_t * restrict xc,
	const xcb_window_t w, const xcb_gc_t gc, const xcb_gc_t bg_gc,
	const uint16_t start, const uint16_t end, const uint8_t pct)
{
	xcb_rectangle_t rect;
	set_rectangle(&rect, start, end);
	// clear:
	xcb_poly_fill_rectangle(xc, w, bg_gc, 1, &rect);
	draw_percent_full(xc, w, gc, rect, pct);

}
void xstatus_draw_battery(xcb_connection_t * xc, const uint16_t start,
	const uint16_t end)
{
	static xcb_gc_t gc[BATTERY_GC_SIZE];
	const xcb_window_t w = xstatus_get_window(xc);
	if (!*gc)
		init_gcs(xc, w, gc);
	{ // pct scope
		const int8_t pct = get_percent();
		if (pct < 0) {// error getting percent
			LOG("Coult not get percent, returning");
			return;
		}
		{ // gc_index scope
			const uint8_t gc_index = get_gc(pct);
			draw_rectangles(xc, w, gc[gc_index],
				gc[BATTERY_GC_BACKGROUND],
				start, end, pct);
			draw_percent(xc, gc[gc_index], pct,
				start + (end-start) / 2);
		}
	}
	xcb_flush(xc);
}

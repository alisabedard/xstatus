// Copyright 2016, Jeffrey E. Bedard
#include "battery.h"
#include "config.h"
#include "clock.h"
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
static void draw_percent(struct XData * restrict X, const xcb_gc_t gc,
	const uint8_t pct, const int16_t x)
{
	const uint8_t buf_sz = 7;
	char buf[buf_sz];
	const uint8_t l = snprintf(buf, buf_sz, " %d%% ", pct);
	xcb_image_text_8(X->xcb, l, X->w, gc, x, xstatus_get_font_size().h, buf);
}
void xstatus_draw_battery(struct XData * restrict X, const uint16_t start,
	const uint16_t end)
{
	static xcb_gc_t gc[BATTERY_GC_SIZE];
	if (!*gc) {
		gc[BATTERY_GC_BACKGROUND] = xstatus_get_gc(X,
			XSTATUS_BATTERY_BACKGROUND_COLOR,
			XSTATUS_BATTERY_BACKGROUND_COLOR);
		gc[BATTERY_GC_AC] = xstatus_get_gc(X,
			XSTATUS_BATTERY_AC_COLOR,
			XSTATUS_BATTERY_BACKGROUND_COLOR);
		gc[BATTERY_GC_BATTERY] = xstatus_get_gc(X,
			XSTATUS_BATTERY_BATTERY_COLOR,
			XSTATUS_BATTERY_BACKGROUND_COLOR);
		gc[BATTERY_GC_CRITICAL] = xstatus_get_gc(X,
			XSTATUS_BATTERY_CRITICAL_COLOR,
			XSTATUS_BATTERY_BACKGROUND_COLOR);
	}
	const uint8_t pct = get_percent();
	const enum BATGCs a = get_gc(pct);
	xcb_rectangle_t g = {.x=start, .y = XSTATUS_CONST_HEIGHT >> 2,
		.height = XSTATUS_CONST_HEIGHT >> 1, .width = end - start
			- XSTATUS_CONST_PAD};
	++g.y;
	xcb_poly_fill_rectangle(X->xcb, X->w,
		gc[BATTERY_GC_BACKGROUND], 1, &g);
	g.width = g.width * pct / 100;
	xcb_poly_fill_rectangle(X->xcb, X->w, gc[a], 1, &g);
	draw_percent(X, gc[a], pct, start + (end-start)/2);
	xcb_flush(X->xcb);
}

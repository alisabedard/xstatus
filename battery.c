// Copyright 2016, Jeffrey E. Bedard

#include "battery.h"
#include "config.h"
#include "clock.h"
#include "xstatus.h"
#include "util.h"

#include "libjb/log.h"
#include "libjb/util.h"

//#define TEST
// get percent value, maxed to 100
static uint8_t get_percent(void)
{
#ifdef TEST
	static int8_t p = 100;
	p -= 10;
	if (p < 0)
		return p = 100;
	return p;
#else//!TEST
	const uint8_t pct = sysval(BATSYSFILE);
	LOG("Percent: %d\n", pct);
	return MIN(pct, 100);
#endif//TEST
}

// index into gc array, keeps gc array private
enum BATGCs { GC_BG, GC_AC, GC_BAT, GC_CRIT, GC_SZ };

// Selects a gc to use based on ac/battery status
static enum BATGCs get_gc(const uint8_t pct)
{
	return sysval(ACSYSFILE) ? GC_AC : pct < CRIT_PCT ? GC_CRIT : GC_BAT;
}

void draw_battery(XData * restrict X, const uint16_t start,
	const uint16_t end)
{
	static xcb_gc_t gc[GC_SZ];
	if (!*gc) {
		gc[GC_BG] = xcbgc(X, PANEL_BG, PANEL_BG);
		gc[GC_AC] = xcbgc(X, GOOD, PANEL_BG);
		gc[GC_BAT] = xcbgc(X, DEGRADED, PANEL_BG);
		gc[GC_CRIT] = xcbgc(X, CRITICAL, PANEL_BG);
	}
	const uint8_t pct = get_percent();
	const enum BATGCs a = get_gc(pct);
	xcb_rectangle_t g = {.x=start, .y = HEIGHT >> 2,
		.height = HEIGHT >> 1, .width = end - start - PAD};
	xcb_poly_fill_rectangle(X->xcb, X->w, gc[GC_BG], 1, &g);
	g.width = g.width * pct / 100;
	xcb_poly_fill_rectangle(X->xcb, X->w, gc[a], 1, &g);
	const uint8_t buf_sz = 7;
	char buf[buf_sz];
	const uint8_t l = snprintf(buf, buf_sz, " %d%% ", pct);
	xcb_image_text_8(X->xcb, l, X->w, gc[a], start + (end-start)/2,
		X->font_height, buf);
	xcb_flush(X->xcb);
}


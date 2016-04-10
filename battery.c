#include "config.h"
#include "log.h"
#include "xstatus.h"
#include "util.h"

static GC bat_bat_gc, bat_ac_gc, bat_crit_gc, invert_gc, bg_gc;
static void setup_gcs(Display *d, Window w)
{
	if(bat_bat_gc) return;
	bat_bat_gc=colorgc(d, w, DEGRADED);
	bat_ac_gc=colorgc(d, w, GOOD);
	bat_crit_gc=colorgc(d, w, CRITICAL);
	XGCValues i = {.function=GXinvert, .font=xstatus_font->fid};
	invert_gc=XCreateGC(d, w, GCFont | GCFunction, &i);
	bg_gc=colorgc(d, w, PANEL_BG);
}

static uint8_t get_percent(void)
{
	uint8_t pct=sysval(BATSYSFILE);
	if(pct>100) {
		pct=100;
	}
	LOG("Percent: %d\n", pct);
	return pct;
}

static void draw_percent(Display * restrict d, const Window w,
	const GC gc, const uint16_t x, const uint16_t width,
	const uint8_t pct)
{
	uint8_t sl=5; // 3 for value, 1 for \%, 1 for \0
	char str_pct[sl];
	sl=snprintf(str_pct, sl, "%d%%", pct);
	const uint16_t center = x+(width>>1);
	XFillRectangle(d, w, bg_gc, center-PAD, 0,
		string_width(sl), HEIGHT);
	XDrawString(d, w, gc, center, font_y(), str_pct, sl);
}

void draw_battery(Display *d, const Window w)
{
	setup_gcs(d, w);
	GC gc=bat_bat_gc;
	const bool on_ac=sysval(ACSYSFILE);
	if(on_ac) gc=bat_ac_gc;
	const uint8_t pct = get_percent();
	if(!on_ac && pct < CRIT_PCT)
		  gc=bat_crit_gc;
	const uint16_t x = xstatus_status_w,
	      width = xstatus_clock_x-x-BUTTON_SPACE;
	XDrawRectangle(d, w, gc, x, 4, width, 10);
	{
		const float filled = width * pct / 100;
		LOG("filled: %f\n", filled);
		XFillRectangle(d, w, gc, x, 5, filled, 9);
	}
	draw_percent(d, w, gc, x, width, pct);
}


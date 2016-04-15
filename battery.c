#include "battery.h"
#include "config.h"
#include "clock.h"
#include "log.h"
#include "xstatus.h"
#include "util.h"

static void setup_gcs(Battery * restrict b)
{
	Display * restrict d = b->widget.d;
	const Window w = b->widget.window;
	b->gc.ac=colorgc(d, w, GOOD);
	b->gc.bat=colorgc(d, w, DEGRADED);
	b->gc.crit=colorgc(d, w, CRITICAL);
	b->gc.bg=colorgc(d, w, PANEL_BG);
}

static uint8_t get_percent(void)
{
	uint8_t pct=sysval(BATSYSFILE);
	LOG("Percent: %d\n", pct);
	return pct>100?100:pct;
}

static void draw_percent(Battery * restrict b)
{
	uint8_t sl=5; // 3 for value + 1 for \% + 1 for \0
	char str_pct[sl];
	sl=snprintf(str_pct, sl, "%d%%", b->pct);
	const Widget * w = &b->widget;
	const uint16_t center = w->geometry.x +(w->geometry.width>>1);
	XFillRectangle(w->d, w->window, b->gc.bg, center-PAD, 0,
		string_width(sl), HEIGHT);
	XDrawString(w->d, w->window, b->widget.gc, center,
		font_y(), str_pct, sl);
}

static void fill(Battery * restrict b)
{
	const Widget * restrict w = &b->widget;
	const float filled = w->geometry.width * b->pct / 100;
	LOG("filled: %f\n", filled);
	XFillRectangle(w->d, w->window, w->gc, w->geometry.x,
		w->geometry.y, filled, w->geometry.height);
}

/* Compute gadget geometry based on available space.  */
static void setup_geometry(Battery * restrict b)
{
	XRectangle * g = &b->widget.geometry;
	g->x = xstatus_status_w;
	g->height = HEIGHT-PAD;
	g->y = PAD/2;
	g->width = xstatus_clock_x - g->x - PAD;
	LOG("setup_geometry(): %dx%d+%d+%d\n",
		g->width, g->height, g->x, g->y);
}

/* Selects a gc to use based on ac/battery status, assigns it to b->widget.gc
 * and returns it.  */
static GC get_gc(Battery * restrict b)
{
	GC gc=b->gc.bat;
	const bool on_ac=sysval(ACSYSFILE);
	if(on_ac) gc=b->gc.ac;
	if(!on_ac && b->pct < CRIT_PCT)
		  gc=b->gc.crit;
	b->widget.gc=gc;
	return gc;
}

static void draw(Battery * restrict b)
{
	b->pct = get_percent();
	GC gc = get_gc(b);
	setup_geometry(b);
	Widget * w = &b->widget;
	XDrawRectangle(w->d, w->window, gc,
		w->geometry.x, w->geometry.y,
		w->geometry.width, w->geometry.height);
	fill(b);
	draw_percent(b);
}

void setup_battery(Battery * b, Display * restrict d, const Window parent)
{
	b->widget.d=d;
	/* Battery is a "gadget", so the parent and the Battery window are one
 	 * and the same.  */
	b->widget.window=b->widget.parent_window=parent;
	setup_gcs(b);
	b->draw=&draw;
}


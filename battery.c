// Copyright 2016, Jeffrey E. Bedard

#include "battery.h"
#include "config.h"
#include "clock.h"
#include "log.h"
#include "xstatus.h"
#include "util.h"

static void setup_gcs(Battery * restrict b)
{
	XData * X = b->widget.X;
	b->gc.ac=colorgc(X, GOOD);
	b->gc.bat=colorgc(X, DEGRADED);
	b->gc.crit=colorgc(X, CRITICAL);
	b->gc.bg=colorgc(X, PANEL_BG);
}

static uint8_t get_percent(void)
{
	uint8_t pct=sysval(BATSYSFILE);
	LOG("Percent: %d\n", pct);
	return pct>100?100:pct;
}

static void draw_percent(Battery * restrict b, const GC gc)
{
	uint8_t sl=5; // 3 for value + 1 for \% + 1 for \0
	char str_pct[sl];
	sl=snprintf(str_pct, sl, "%d%%", b->pct);
	const Widget * w = &b->widget;
	const uint16_t center = w->geometry.x
		+ (w->geometry.width>>1);
	XFillRectangle(w->X->d, w->window, b->gc.bg,
		center-PAD, 0, w->X->font_width * sl + PAD + PAD,
		HEIGHT);
	XDrawString(w->X->d, w->window, gc, center,
		w->X->font_height, str_pct, sl);
}

static void fill(Battery * restrict b, const GC gc)
{
	const Widget * restrict w = &b->widget;
	const float filled = w->geometry.width * b->pct / 100;
	LOG("filled: %f\n", filled);
	XFillRectangle(w->X->d, w->window, gc, w->geometry.x,
		w->geometry.y, filled, w->geometry.height);
}

/* Compute gadget geometry based on available space.  */
static void setup_geometry(Battery * restrict b)
{
	xcb_rectangle_t * restrict g = &b->widget.geometry;
	g->x = b->x.begin;
	g->height = HEIGHT/2;
	g->y = HEIGHT/4;
	g->width = b->x.end - g->x - PAD;
	LOG("setup_geometry(): %dx%d+%d+%d\n",
		g->width, g->height, g->x, g->y);
}

/* Selects a gc to use based on ac/battery status,
   assigns it to b->widget.gc and returns it.  */
static GC get_gc(Battery * restrict b)
{
	return sysval(ACSYSFILE) ? b->gc.ac : b->pct
		< CRIT_PCT ? b->gc.crit : b->gc.bat;
}

static void draw(Battery * restrict b)
{
	b->pct = get_percent();
	GC gc = get_gc(b);
	setup_geometry(b);
	Widget * w = &b->widget;
	XDrawRectangle(w->X->d, w->window, gc,
		w->geometry.x, w->geometry.y,
		w->geometry.width, w->geometry.height);
	fill(b, gc);
	draw_percent(b, gc);
}

void setup_battery(Battery * restrict b, XData * restrict X)
{
	b->widget.X=X;
	/* Battery is a "gadget", so the parent and the
	   Battery window are identical.  */
	b->widget.window=X->w;
	setup_gcs(b);
	b->draw=&draw;
}


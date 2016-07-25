// Copyright 2016, Jeffrey E. Bedard

#include "battery.h"
#include "config.h"
#include "clock.h"
#include "xstatus.h"
#include "util.h"

#include "libjb/log.h"

#include <assert.h>

static void setup_gcs(Battery * restrict b)
{
	XData * X = b->widget.X;
	b->gc.ac = xcbgc(X, GOOD, PANEL_BG);
	b->gc.bat = xcbgc(X, DEGRADED, PANEL_BG);
	b->gc.crit = xcbgc(X, CRITICAL, PANEL_BG);
	b->gc.bg = xcbgc(X, PANEL_BG, PANEL_BG);
}

static uint8_t get_percent(void)
{
	uint8_t pct=sysval(BATSYSFILE);
	LOG("Percent: %d\n", pct);
	return pct > 100 ? 100 : pct;
}

static void draw_percent(Battery * restrict b,
	const xcb_gc_t gc)
{
	// 3 for value + 1 for \% + 1 for \0
#define BUFSZ 5
	static char str_pct[BUFSZ];
	const uint8_t sl = snprintf(str_pct, BUFSZ, "%d%%", b->pct);
	const Widget * w = &b->widget;
	const uint16_t center = w->geometry.x
		+ (w->geometry.width >> 1);
	XData * X = w->X;
	xcb_poly_fill_rectangle(X->xcb, w->window, b->gc.bg, 1,
		&(xcb_rectangle_t){.x = center - PAD,
		.width = X->font_width * sl + PAD + PAD,
		.height = HEIGHT});
	xcb_image_text_8(X->xcb, sl, w->window, gc, center,
		X->font_height, str_pct);
}

static void fill(Battery * restrict b, const xcb_gc_t gc)
{
	const Widget * restrict w = &b->widget;
	xcb_rectangle_t r = w->geometry;
	XData * X = w->X;
	xcb_connection_t * c = X->xcb;
	const xcb_window_t win = w->window;
	xcb_poly_fill_rectangle(c, win, gc, 1, &r);
	const uint8_t pct = b->pct;
	if (pct >= 100) // no need to clear, nor to overflow type.
		return;
	const uint16_t filled = r.width * b->pct / 100;
	// Adjust to fit inside outline.
	r.x += filled + 1;
	r.width -= filled;
	r.width -= 2;
	++r.y;
	r.height -= 2;
	xcb_clear_area(c, 0, win, r.x, r.y, r.width, r.height);
	xcb_flush(c);
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
static xcb_gc_t get_gc(Battery * restrict b)
{
	return sysval(ACSYSFILE) ? b->gc.ac : b->pct
		< CRIT_PCT ? b->gc.crit : b->gc.bat;
}

static void draw(Battery * restrict b)
{
	b->pct = get_percent();
	xcb_gc_t gc = get_gc(b);
	setup_geometry(b);
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


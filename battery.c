#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include "config.h"
#include "xstatus.h"
#include "util.h"

static GC bat_bat_gc, bat_ac_gc, bat_crit_gc;
static void setup_gcs(Display *d, Window w)
{
	if(bat_bat_gc) return;
	bat_bat_gc=colorgc(d, w, BAT_BAT);
	bat_ac_gc=colorgc(d, w, BAT_AC);
	bat_crit_gc=colorgc(d, w, BAT_CRIT);
}

void draw_battery(Display *d, const Window w)
{
	setup_gcs(d, w);
	GC gc=bat_bat_gc;
	const bool on_ac=sysval(ACSYSFILE);
	if(on_ac) gc=bat_ac_gc;
	int pct=sysval(BATSYSFILE);
#ifdef DEBUG
	printf("Percent: %d\n", pct);
#endif//DEBUG
	if(!on_ac && pct < CRIT_PCT)
		  gc=bat_crit_gc;
	const uint16_t x = xstatus_status_w;
	const uint16_t width = xstatus_clock_x-x-BUTTON_SPACE;
	XDrawRectangle(d, w, gc, x, 4, width, 10);
	if(pct>100) pct=100;
	const float p=width*pct/100;
#ifdef DEBUG
	printf("p: %f\n", p);
#endif//DEBUG:
	XFillRectangle(d, w, gc, x, 5, p, 9);
}


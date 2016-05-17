#ifndef BATTERY_H
#define BATTERY_H

#ifdef USE_BATTERY
#include <X11/Xlib.h>
#include "widget.h"
#include "xdata.h"

typedef struct Battery {
	uint8_t pct;
	struct {
		uint16_t begin, end;
	} x;
	struct {
		GC ac, bat, crit, bg;
	} gc;
	Widget widget;
	void (*draw)(struct Battery *);
} Battery;

void setup_battery(Battery * restrict b, XData * restrict X);

#else//!USE_BATTERY
#define setup_battery(b, X)
#endif//USE_BATTERY

#endif//!BATTERY_H

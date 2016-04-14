#ifndef BATTERY_H
#define BATTERY_H

#include <X11/Xlib.h>
#include "widget.h"

typedef struct Battery {
	uint8_t pct;
	struct {
		GC ac, bat, crit, bg;
	} gc;
	Widget widget;
	void (*draw)(struct Battery *);
} Battery;

void setup_battery(Battery * b, Display * restrict d,
	const Window parent);

#endif//!BATTERY_H

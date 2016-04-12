#ifndef XS_WIDGET_H
#define XS_WIDGET_H

#include <stdint.h>
#include <X11/Xlib.h>
#include "util.h"

typedef struct Widget {
	Display *d;
	Window window, parent_window;
	GC gc;
	XRectangle geometry;
} Widget;

void create_widget(Widget * restrict w, Display * restrict d, 
	const Window parent, XRectangle *geometry, const uint8_t border_width, 
	const Pixel border_color, const Pixel background_color,
	const GC gc);

#endif//!XS_WIDGET_H

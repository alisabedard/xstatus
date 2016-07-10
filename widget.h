#ifndef XS_WIDGET_H
#define XS_WIDGET_H

#include <stdint.h>
#include <X11/Xlib.h>
#include "util.h"
#include "xdata.h"

typedef struct Widget {
	XData * X; // X->w is the parent window.
	xcb_rectangle_t geometry;
	xcb_window_t window;
} Widget;

void setup_Widget(Widget * w, XData * restrict X,
	xcb_rectangle_t * restrict geometry, const Pixel bg);

#endif//!XS_WIDGET_H

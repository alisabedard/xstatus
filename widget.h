#ifndef XS_WIDGET_H
#define XS_WIDGET_H

#include <stdint.h>
#include <X11/Xlib.h>
#include "util.h"
#include "xdata.h"

typedef struct Widget {
	Window window;
	XData * X; // X->w is the parent window.  
	XRectangle geometry;
} Widget;

void setup_Widget(Widget * w, XData * restrict X,
	XRectangle * restrict geometry, const Pixel bg);

#endif//!XS_WIDGET_H

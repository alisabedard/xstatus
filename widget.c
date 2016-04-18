#include <assert.h>
#include <string.h>
#include <X11/Xlib.h>
#include "widget.h"
#include "util.h"

void setup_Widget(Widget * w, XData * restrict X, XRectangle * restrict geometry,
	const Pixel bg)
{
	memcpy(&w->geometry, geometry, sizeof(XRectangle));
	w->window=XCreateSimpleWindow(X->d, X->w, geometry->x, geometry->y,
		geometry->width, geometry->height, 0, bg, bg);
	XMapWindow(X->d, w->window);
	w->X=X;
}


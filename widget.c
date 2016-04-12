#include <assert.h>
#include <string.h>
#include <X11/Xlib.h>
#include "widget.h"
#include "util.h"

void create_widget(Widget * restrict w, Display * restrict d, 
	const Window parent, XRectangle *geometry, const uint8_t border_width, 
	const Pixel border_color, const Pixel background_color,
	const GC gc)
{
	w->parent_window=parent;
	w->d=d;
	w->gc=gc;
	memcpy(&w->geometry, geometry, sizeof(XRectangle));
	w->window=XCreateSimpleWindow(d, parent, geometry->x, geometry->y,
		geometry->width, geometry->height, border_width,
		border_color, background_color);
	assert(w->window);
	XMapWindow(d, w->window);
}


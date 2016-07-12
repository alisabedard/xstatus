#include <assert.h>
#include <string.h>
#include <X11/Xlib.h>
#include "widget.h"
#include "util.h"

void setup_Widget(Widget * w, XData * restrict X,
	xcb_rectangle_t * restrict geometry,
	const Pixel bg, const uint32_t events)
{
	w->window = xcb_generate_id(X->xcb);
	memcpy(&w->geometry, geometry, sizeof(xcb_rectangle_t));
	w->X=X;
	xcb_create_window(X->xcb, X->screen->root_depth,
		w->window, X->w, geometry->x, geometry->y,
		geometry->width, geometry->height, 0,
		XCB_WINDOW_CLASS_COPY_FROM_PARENT,
		X->screen->root_visual, XCB_CW_BACK_PIXEL
		| XCB_CW_EVENT_MASK, (uint32_t[]){bg, events});
	xcb_map_window(X->xcb, w->window);
}


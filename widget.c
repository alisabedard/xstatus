// Copyright 2016, Jeffrey E. Bedard
#include "widget.h"
void setup_Widget(struct Widget * w, struct XData * restrict X,
	xcb_rectangle_t * restrict geometry,
	const pixel_t bg, const uint32_t events)
{
	w->window = xcb_generate_id(X->xcb);
	w->geometry = *geometry;
	w->X=X;
	const uint32_t vm = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	xcb_create_window(X->xcb, XCB_COPY_FROM_PARENT,
		w->window, X->w, geometry->x, geometry->y,
		geometry->width, geometry->height, 0,
		XCB_WINDOW_CLASS_COPY_FROM_PARENT,
		XCB_COPY_FROM_PARENT, vm, (uint32_t[]){bg, events});
	xcb_map_window(X->xcb, w->window);
}

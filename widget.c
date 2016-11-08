// Copyright 2016, Jeffrey E. Bedard
#include "widget.h"
#include "xdata.h"
#include <stdlib.h>
struct XStatusWidget * xstatus_get_widget(
	xcb_connection_t * restrict xc,
	struct XStatusWidget * restrict w,
	xcb_rectangle_t * restrict geometry,
	const pixel_t bg, const uint32_t events)
{
	if (!w)
		w = malloc(sizeof(struct XStatusWidget));
	const xcb_window_t win = w->window = xcb_generate_id(xc);
	w->geometry = *geometry;
	const uint32_t vm = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	xcb_create_window(xc, XCB_COPY_FROM_PARENT,
		win, xstatus_get_window(xc), geometry->x, geometry->y,
		geometry->width, geometry->height, 0,
		XCB_WINDOW_CLASS_COPY_FROM_PARENT,
		XCB_COPY_FROM_PARENT, vm, (uint32_t[]){bg, events});
	xcb_map_window(xc, win);
	return w;
}

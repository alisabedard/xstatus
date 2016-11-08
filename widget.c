// Copyright 2016, Jeffrey E. Bedard
#include "widget.h"
#include "xdata.h"
#include <stdlib.h>
void xstatus_create_widget_window(xcb_connection_t * restrict xc,
	const xcb_window_t window, xcb_rectangle_t * restrict geo,
	const pixel_t bg, const uint32_t events)
{
	xcb_create_window(xc, XCB_COPY_FROM_PARENT, window,
		xstatus_get_window(xc), geo->x, geo->y, geo->width,
		geo->height, 0, XCB_WINDOW_CLASS_COPY_FROM_PARENT,
		XCB_COPY_FROM_PARENT, XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK,
		(uint32_t[]){bg, events});
	xcb_map_window(xc, window);

}
struct XStatusWidget * xstatus_get_widget(
	xcb_connection_t * restrict xc,
	struct XStatusWidget * restrict w,
	xcb_rectangle_t * restrict geometry,
	const pixel_t bg, const uint32_t events)
{
	if (!w)
		w = malloc(sizeof(struct XStatusWidget));
	const xcb_window_t win = w->window = xcb_generate_id(xc);
	xstatus_create_widget_window(xc, win, geometry, bg, events);
	w->geometry = *geometry;
	return w;
}

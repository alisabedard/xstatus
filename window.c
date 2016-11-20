// Copyright 2016, Jeffrey E. Bedard
#include "window.h"
#include "config.h"
#include "libjb/xcb.h"
#include "xdata.h"
static inline int16_t get_y(xcb_screen_t * restrict s)
{
	return s->height_in_pixels - XSTATUS_CONST_HEIGHT
		- XSTATUS_CONST_BORDER;
}
static pixel_t get_bg(xcb_connection_t * restrict xc,
	xcb_screen_t * restrict s)
{
	return jb_get_pixel(xc, s->default_colormap,
		XSTATUS_PANEL_BACKGROUND);
}
static xcb_rectangle_t get_geometry(xcb_screen_t * restrict s)
{
	return (xcb_rectangle_t){0, get_y(s), s->width_in_pixels,
			XSTATUS_CONST_HEIGHT};
}
void xstatus_create_window(xcb_connection_t * restrict xc)
{
	const xcb_window_t w = xstatus_get_window(xc);
	{ // * s, em, vm, and g scope
		enum {vm = XCB_CW_BACK_PIXEL | XCB_CW_OVERRIDE_REDIRECT
			| XCB_CW_EVENT_MASK, em = XCM_EVENT_MASK_EXPOSURE};
		xcb_screen_t * s = xstatus_get_screen(xc);
		const xcb_rectangle_t g = get_geometry(s);
		xcb_create_window(xc, XCB_COPY_FROM_PARENT, w, s->root, g.x, g.y,
			g.width, g.height, XSTATUS_CONST_BORDER,
			XCB_WINDOW_CLASS_COPY_FROM_PARENT, XCB_COPY_FROM_PARENT,
			vm, (uint32_t[]){get_bg(xc, s), true, em});
	}
	xcb_map_window(xc, w);
}

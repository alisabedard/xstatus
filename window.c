// Copyright 2016, Jeffrey E. Bedard
#include "window.h"
#include "config.h"
#include "libjb/xcb.h"
#include "xdata.h"
__attribute__((const))
static inline int16_t get_y(const uint16_t screen_height)
{
	return screen_height - XSTATUS_CONST_HEIGHT - XSTATUS_CONST_BORDER;
}
__attribute__((nonnull))
static inline pixel_t get_bg(xcb_connection_t * restrict xc,
	xcb_screen_t * restrict s)
{
	return jb_get_pixel(xc, s->default_colormap,
		XSTATUS_PANEL_BACKGROUND);
}
__attribute__((nonnull))
static inline xcb_rectangle_t get_geometry(xcb_screen_t * restrict s)
{
	return (xcb_rectangle_t){0, get_y(s->height_in_pixels),
		s->width_in_pixels, XSTATUS_CONST_HEIGHT};
}
__attribute__((nonnull))
void xstatus_create_window(xcb_connection_t * restrict xc)
{
	const xcb_window_t w = xstatus_get_window(xc);
	{ // * s, em, vm, and g scope
		enum {
			VM = XCB_CW_BACK_PIXEL | XCB_CW_OVERRIDE_REDIRECT
				| XCB_CW_EVENT_MASK,
			EM = XCB_EVENT_MASK_EXPOSURE,
			CFP = XCB_COPY_FROM_PARENT
		};
		xcb_screen_t * s = xstatus_get_screen(xc);
		const xcb_rectangle_t g = get_geometry(s);
		xcb_create_window(xc, CFP, w, s->root, g.x, g.y, g.width,
			g.height, XSTATUS_CONST_BORDER, CFP, CFP,
			VM, (uint32_t[]){get_bg(xc, s), true, EM});
	}
	xcb_map_window(xc, w);
}

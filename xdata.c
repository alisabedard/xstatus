// Copyright 2016, Jeffrey E. Bedard
#include "xdata.h"
#include "libjb/xcb.h"
xcb_screen_t * xstatus_get_screen(xcb_connection_t * xc)
{
	static xcb_screen_t * s;
	return s ? s : (s = jb_get_xcb_screen(xc));
}
xcb_colormap_t xstatus_get_colormap(xcb_connection_t * xc)
{
	static xcb_colormap_t c;
	return c ? c : (c = xstatus_get_screen(xc)->default_colormap);
}
xcb_gcontext_t xstatus_get_gc(xcb_connection_t * xc)
{
	static xcb_gcontext_t g;
	return g ? g : (g = xcb_generate_id(xc));
}
xcb_gcontext_t xstatus_get_button_gc(xcb_connection_t * xc)
{
	static xcb_gcontext_t g;
	return g ? g : (g = xcb_generate_id(xc));
}
xcb_gcontext_t xstatus_get_invert_gc(xcb_connection_t * xc)
{
	static xcb_gcontext_t g;
	return g ? g : (g = xcb_generate_id(xc));
}

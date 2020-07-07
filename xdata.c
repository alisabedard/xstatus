// Copyright 2017, Jeffrey E. Bedard
#include "xdata.h"
#include "libjb/xcb.h"
void init_XSXData(xcb_connection_t * xc, struct XSXData * i) {
  i->xc = xc;
  i->screen = jb_get_xcb_screen(xc);
  i->colormap = i->screen->default_colormap;
  i->font = xcb_generate_id(xc);
  i->alert_gc = xcb_generate_id(xc);
  i->button_gc = xcb_generate_id(xc);
  i->gc = xcb_generate_id(xc);
  i->invert_gc = xcb_generate_id(xc);
  i->window = xcb_generate_id(xc);
}
#define XDATA_CACHED(type, name, value) type name(xcb_connection_t * xc) \
	{ static type a; return a ? a : (a = value); }
#define XDATA_GETID(type, name) XDATA_CACHED(type, name, xcb_generate_id(xc))
XDATA_CACHED(xcb_screen_t *, xstatus_get_screen, jb_get_xcb_screen(xc));
XDATA_CACHED(xcb_colormap_t, xstatus_get_colormap,
	xstatus_get_screen(xc)->default_colormap);
XDATA_GETID(xcb_gcontext_t, xstatus_get_gc);
XDATA_GETID(xcb_gcontext_t, xstatus_get_button_gc);
XDATA_GETID(xcb_gcontext_t, xstatus_get_invert_gc);
XDATA_GETID(xcb_window_t,  xstatus_get_window);
XDATA_GETID(xcb_font_t,  xstatus_get_font);

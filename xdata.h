// Copyright 2017, Jeffrey E. Bedard
#ifndef XS_XDATA_H
#define XS_XDATA_H
#include <xcb/xcb.h>
xcb_colormap_t xstatus_get_colormap(xcb_connection_t * xc);
xcb_gcontext_t xstatus_get_button_gc(xcb_connection_t * xc);
xcb_gcontext_t xstatus_get_gc(xcb_connection_t * xc);
xcb_gcontext_t xstatus_get_invert_gc(xcb_connection_t * xc);
xcb_screen_t * xstatus_get_screen(xcb_connection_t * xc);
xcb_window_t xstatus_get_window(xcb_connection_t * xc);
xcb_font_t xstatus_get_font(xcb_connection_t * xc);
#endif//!XS_XDATA_H

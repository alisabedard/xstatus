// Copyright 2017, Alisa Bedard
#ifndef XS_XDATA_H
#define XS_XDATA_H
#include "XSFont.h"
#include <xcb/xcb.h>
struct XSXData {
  struct XSFont FontData;
  xcb_connection_t * xc;
  xcb_screen_t * screen;
  xcb_colormap_t colormap;
  xcb_gcontext_t alert_gc, button_gc, gc, invert_gc, foreground, background;
  xcb_window_t window;
};
void init_XSXData(xcb_connection_t * xc, struct XSXData * i);
#endif//!XS_XDATA_H

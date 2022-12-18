// Copyright 2017, Alisa Bedard
#include "XSXData.h"
#include "libjb/xcb.h"
void init_XSXData(xcb_connection_t * xc, struct XSXData * i) {
  i->xc = xc;
  i->screen = jb_get_xcb_screen(xc);
  i->colormap = i->screen->default_colormap;
  i->FontData.Font = xcb_generate_id(xc);
  i->alert_gc = xcb_generate_id(xc);
  i->button_gc = xcb_generate_id(xc);
  i->gc = xcb_generate_id(xc);
  i->invert_gc = xcb_generate_id(xc);
  i->window = xcb_generate_id(xc);
}

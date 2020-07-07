// Copyright 2017, Jeffrey E. Bedard
#include "window.h"
#include "config.h"
#include "libjb/xcb.h"
#include "XSXData.h"
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
void xstatus_create_window(struct XSXData * restrict x)
{
  enum {
    VM = XCB_CW_BACK_PIXEL | XCB_CW_OVERRIDE_REDIRECT
    | XCB_CW_EVENT_MASK,
    EM = XCB_EVENT_MASK_EXPOSURE,
    CFP = XCB_COPY_FROM_PARENT
  };
  const xcb_rectangle_t g = get_geometry(x->screen);
  xcb_create_window(x->xc, CFP, x->window, x->screen->root,
    g.x, g.y, g.width, g.height, XSTATUS_CONST_BORDER, CFP, CFP,
    VM, (uint32_t[]){get_bg(x->xc, x->screen), true, EM});
  xcb_map_window(x->xc, x->window);
}

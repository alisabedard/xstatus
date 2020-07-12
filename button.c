// Copyright 2017, Jeffrey E. Bedard
#include "button.h"
#include <stdlib.h>
#include <string.h>
#include "XSTextWidget.h"
#include "config.h"
#include "font.h"
#include "libjb/xcb.h"
#include "text_widget.h"
#include "XSXData.h"
static void draw(struct XSButton * restrict b)
{
  xcb_connection_t * xc = b->widget.X->xc;
  xcb_image_text_8(xc, b->length, b->widget.Window, b->widget.X->gc,
    XSTATUS_CONST_PAD, b->font_size.height, b->label);
}
static void invert(struct XSButton * restrict b)
{
  struct XSXData *X = b->widget.X;
  xcb_connection_t * xc = X->xc;
  xcb_poly_fill_rectangle(xc, b->widget.Window, X->invert_gc,
    1, &(xcb_rectangle_t){0, 0, b->widget.Geometry[2], b->font_size.height});
  xcb_flush(xc);
}
static pixel_t get_bg(struct XSXData * X)
{
  static pixel_t p; // cache the result
  return p ? p : (p = jb_get_pixel(X->xc, X->colormap, XSTATUS_BUTTON_BG));
}
static inline uint16_t get_width(uint8_t fw, const char * label)
{
  return fw * strlen(label) + fw;
}
static inline uint8_t get_height(uint8_t fh)
{
  return fh + (XSTATUS_CONST_PAD >> 1);
}
static xcb_rectangle_t get_geometry(struct XSButton * b)
{
  return (xcb_rectangle_t){.x = b->widget.Geometry[0],
    .y = b->widget.Geometry[1] = 0,
    .width = b->widget.Geometry[2] =
    get_width(b->font_size.width, b->label), .height =
    b->widget.Geometry[3] = get_height(b->font_size.height)};
}
static void create_window(struct XSButton * b)
{
  struct XSXData * X = b->widget.X;
  const xcb_window_t w = b->widget.Window;
  xcb_connection_t * restrict xc = b->widget.X->xc;
  { // g scope, vm scope, em scope
    enum {
      VM = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK,
      EM = XCB_EVENT_MASK_EXPOSURE
      | XCB_EVENT_MASK_BUTTON_PRESS
      | XCB_EVENT_MASK_ENTER_WINDOW
      | XCB_EVENT_MASK_LEAVE_WINDOW,
      CFP = XCB_COPY_FROM_PARENT,
      BORDER = 0
    };
    const xcb_rectangle_t g = get_geometry(b);
    xcb_create_window(xc, CFP, w, X->window,
      g.x, g.y, g.width, g.height, BORDER,
      CFP, CFP, VM, (uint32_t[]){get_bg(X), EM});
  }
  xcb_map_window(xc, w);
}

struct XSButton * createButton(struct XSXData * restrict XData,
  char * Label, int16_t const XPosition) {
  struct XSButton * b = calloc(1, sizeof(struct XSButton));
  b->widget.X = XData;
  b->widget.Window = xcb_generate_id(XData->xc);
  b->label = Label;
  b->length = strlen(Label);
  b->draw = draw;
  b->enter = invert;
  b->leave = invert;
  b->font_size = xstatus_get_font_size();
  b->widget.Geometry[0] = XPosition;
  create_window(b);
  draw(b);
  return b;
}

// Copyright 2017, Jeffrey E. Bedard
#include "xstatus.h"
#include "battery.h"
#include "clock.h"
#include "config.h"
#include "font.h"
#include "libjb/log.h"
#include "libjb/xcb.h"
#include "load.h"
#include "status_file.h"
#include "temperature.h"
#include "toolbar.h"
#include "util.h"
#include "window.h"
#include "XSXData.h"
static uint16_t poll_status(struct XSXData * restrict x,
  const char * filename, const uint16_t widget_start)
{
  uint16_t offset = widget_start + XSTATUS_CONST_PAD;
  offset = xstatus_draw_load(x, offset);
  offset = draw_temp(x, offset);
  offset = draw_status_file(x, offset, filename);
  return offset;
}
static void update(struct XSXData * restrict x,
  const char * restrict filename, const uint16_t widget_start)
{
  xstatus_draw_battery(x, (struct JBDim){.start = poll_status(x,
      filename, widget_start), .end = xstatus_draw_clock(x)});
}
// returns if update needed
__attribute__((nonnull))
static void handle_events(struct XSXData * restrict x,
  xcb_generic_event_t * restrict e, const char * restrict filename,
  const uint16_t widget_start)
{
  switch (e->response_type) {
  case XCB_ENTER_NOTIFY:
#if LOG_LEVEL > 8
    LOG("enter");
#endif//LOG_LEVEL
    xstatus_toolbar_handle_button_enter(
      ((xcb_enter_notify_event_t*)e)->event);
    break;
  case XCB_LEAVE_NOTIFY:
#if LOG_LEVEL > 8
    LOG("leave");
#endif//LOG_LEVEL
    xstatus_toolbar_handle_button_leave(
      ((xcb_leave_notify_event_t*)e)->event);
    break;
  case XCB_EXPOSE:
    if (!xstatus_toolbar_handle_expose(((xcb_expose_event_t*)e)
        ->window))
      update(x, filename, widget_start);
    break;
  case XCB_BUTTON_PRESS:
    xstatus_toolbar_handle_button_press(
      ((xcb_button_press_event_t *)e)->event);
    break;
  default:
    LOG("event: %d", e->response_type);
  }
  free(e);
}
void xstatus_create_gc(struct XSXData * restrict x,
  const xcb_gcontext_t gc, const char * restrict fg, const char * restrict bg)
{
  jb_create_gc(x->xc, gc, x->window, fg, bg);
  xcb_change_gc(x->xc, gc, XCB_GC_FONT, &x->font);
}

static void initialize_gcs(struct XSXData * restrict x)
{
  xstatus_create_gc(x, x->gc, 
    XSTATUS_PANEL_FOREGROUND, XSTATUS_PANEL_BACKGROUND);
  xstatus_create_gc(x, x->button_gc, 
    XSTATUS_BUTTON_FG, XSTATUS_BUTTON_BG);
  xcb_create_gc(x->xc, x->invert_gc, x->window, XCB_GC_FUNCTION,
    (uint32_t[]){XCB_GX_INVERT});
}
void xstatus_start(struct XStatusOptions * restrict opt)
{
  struct XSXData X;
  xcb_connection_t * xc = jb_get_xcb_connection(NULL, NULL);
  init_XSXData(xc, &X);
  xstatus_create_window(&X);
  xstatus_open_font(&X, XSTATUS_FONT);
  initialize_gcs(&X);
  const uint16_t start = xstatus_initialize_toolbar(&X);
  for (;;) {
    xcb_generic_event_t * e;
    if (jb_next_event_timed(xc, &e, opt->delay * 1000000) && e)
      handle_events(&X, e, opt->filename, start);
    else
      update(&X, opt->filename, start);

  }
}

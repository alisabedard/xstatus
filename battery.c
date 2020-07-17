// Copyright 2017, Jeffrey E. Bedard
#include "battery.h"
#include <stdio.h>
#include "XSTextWidget.h"
#include "XSWidget.h"
#include "XSXData.h"
#include "config.h"
#include "font.h"
#include "libjb/macros.h"
#include "text_widget.h"
#include "util.h"
#include "xstatus.h"
//#define XSTATUS_BATTERY_TEST
// get percent value, maximum 100, -1 on error
static int8_t get_percent(void)
{
  return JB_MIN(xstatus_system_value(XSTATUS_SYSFILE_BATTERY), 100);
}
// index into gc array, keeps gc array private
enum BATGCs { BATTERY_GC_BACKGROUND, BATTERY_GC_AC, BATTERY_GC_BATTERY,
  BATTERY_GC_CRITICAL, BATTERY_GC_SIZE };
// Selects a gc to use based on ac/battery status
static uint8_t get_gc(const uint8_t pct)
{
  return xstatus_system_value(XSTATUS_SYSFILE_AC) ? BATTERY_GC_AC : pct
  < XSTATUS_CONST_CRITICAL_PERCENT
  ? BATTERY_GC_CRITICAL : BATTERY_GC_BATTERY;
}
static uint8_t format(char * buf, const uint8_t sz, const uint8_t pct)
{
  return snprintf(buf, sz, " %d%% ", pct);
}
static void draw_percent(struct XSWidget * widget, const uint8_t pct)
{
  enum {BUF_SZ = 7};
  char buf[BUF_SZ];
  struct XSTextWidget w = {widget->X, buf, format(buf, BUF_SZ,
    pct), widget->Geometry[0]};
  xstatus_draw_text_widget(&w);
}
static void set_gc(struct XSXData * restrict X,
  xcb_gcontext_t * restrict gc, const char * restrict fg)
{
  xstatus_create_gc(X, *gc = xcb_generate_id(X->xc), fg,
    XSTATUS_BATTERY_BACKGROUND_COLOR);
}
static void initialize_gcs(struct XSWidget * widget, xcb_gcontext_t * gc)
{
#define SETGC(color) set_gc(widget->X, \
  gc + BATTERY_GC_##color, XSTATUS_BATTERY_##color##_COLOR);
  SETGC(BACKGROUND); SETGC(AC); SETGC(BATTERY); SETGC(CRITICAL);
}
static xcb_rectangle_t get_rectangle(const struct JBDim range)
{
  return (xcb_rectangle_t){.x=range.start,
    .y = (XSTATUS_CONST_HEIGHT >> 2) + 1,
    .height = XSTATUS_CONST_HEIGHT >> 1,
    .width = range.end - range.start - XSTATUS_CONST_PAD};
}
static void draw_rectangles(struct XSWidget * widget, const struct JBDim
  range, const uint8_t pct)
{
  xcb_rectangle_t rect = get_rectangle(range);
  const xcb_window_t w = widget->Window;
  xcb_connection_t * xc = widget->X->xc;
  // clear:
  xcb_poly_fill_rectangle(xc, w, widget->X->background, 1, &rect);
  rect.width *= pct / 100.0;
  // fill rectangle per percent full:
  xcb_poly_fill_rectangle(xc, w, widget->X->foreground, 1, &rect);
}
__attribute__((const))
static uint16_t get_x(const struct JBDim range)
{
  return range.start + (range.end - range.start) / 2;
}
static xcb_gcontext_t * get_gcs(struct XSWidget * widget)
{
  static xcb_gcontext_t gc[BATTERY_GC_SIZE];
  if (!*gc)
    initialize_gcs(widget, gc);
  return gc;
}
void drawBattery(struct XSXData * restrict X,
  const struct JBDim range)
{
  const int8_t pct = get_percent();
  if (pct >= 0) {
    struct XSWidget widget = {X, X->window,
      .Geometry = {get_x(range)}};
    xcb_gcontext_t * gcs = get_gcs(&widget);
    widget.X->foreground = gcs[get_gc(pct)];
    widget.X->background = gcs[BATTERY_GC_BACKGROUND];
    draw_rectangles(&widget, range, pct);
    draw_percent(&widget, pct);
    xcb_flush(X->xc);
  }
}

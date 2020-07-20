// Copyright 2017-2020, Jeffrey E. Bedard
#include "battery.h"
#include <stdio.h>
#include "XSTextWidget.h"
#include "XSWidget.h"
#include "config.h"
#include "text_widget.h"
#include "util.h"
#include "xstatus.h"
//#define XSTATUS_BATTERY_TEST
// get percent value, maximum 100, -1 on error
static int8_t get_percent(void)
{
  int const Value = xstatus_system_value(XSTATUS_SYSFILE_BATTERY);
  return Value < 100 ? Value : 100;
}
// index into gc array, keeps gc array private
enum BATGCs { BATTERY_GC_BACKGROUND, BATTERY_GC_AC, BATTERY_GC_BATTERY,
  BATTERY_GC_CRITICAL, BATTERY_GC_SIZE };
// Selects a gc to use based on ac/battery status
static uint8_t getGC(const uint8_t Percent)
{
  return xstatus_system_value(XSTATUS_SYSFILE_AC) ? BATTERY_GC_AC : Percent
  < XSTATUS_CONST_CRITICAL_PERCENT
  ? BATTERY_GC_CRITICAL : BATTERY_GC_BATTERY;
}
static void drawPercent(struct XSWidget * widget, const uint8_t Percent)
{
  enum {BUF_SZ = 7};
  char buf[BUF_SZ];
  struct XSTextWidget w = {widget->X, buf, snprintf(buf, BUF_SZ, " %d%% ",
    Percent), widget->Geometry[0]};
  xstatus_draw_text_widget(&w);
}
static void setGC(struct XSXData * restrict X,
  xcb_gcontext_t * restrict gc, const char * restrict fg)
{
  xstatus_create_gc(X, *gc = xcb_generate_id(X->xc), fg,
    XSTATUS_BATTERY_BACKGROUND_COLOR);
}
static void initializeGCs(struct XSWidget * widget, xcb_gcontext_t * gc)
{
#define SETGC(color) setGC(widget->X, \
  gc + BATTERY_GC_##color, XSTATUS_BATTERY_##color##_COLOR);
  SETGC(BACKGROUND); SETGC(AC); SETGC(BATTERY); SETGC(CRITICAL);
}
static xcb_rectangle_t getRectangle(int16_t const XStart,
  int16_t const XEnd)
{
  return (xcb_rectangle_t){.x=XStart,
    .y = (XSTATUS_CONST_HEIGHT >> 2) + 1,
    .height = XSTATUS_CONST_HEIGHT >> 1,
    .width = XEnd - XStart - XSTATUS_CONST_PAD};
}
static void drawRectangles(struct XSWidget * widget, int16_t const XStart,
  int16_t XEnd, const uint8_t Percent)
{
  xcb_rectangle_t rect = getRectangle(XStart, XEnd);
  const xcb_window_t w = widget->Window;
  xcb_connection_t * xc = widget->X->xc;
  // clear:
  xcb_poly_fill_rectangle(xc, w, widget->X->background, 1, &rect);
  rect.width *= Percent / 100.0;
  // fill rectangle per percent full:
  xcb_poly_fill_rectangle(xc, w, widget->X->foreground, 1, &rect);
}
static xcb_gcontext_t * getGCs(struct XSWidget * widget)
{
  static xcb_gcontext_t gc[BATTERY_GC_SIZE];
  if (!*gc)
    initializeGCs(widget, gc);
  return gc;
}
void drawBattery(struct XSXData * restrict X, int16_t XStart,
  int16_t const XEnd)
{
  const int8_t Percent = get_percent();
  if (Percent >= 0) {

    //  int16_t const range.start + (range.end - range.start) / 2;
    struct XSWidget widget = {X, X->window,
      .Geometry = {
        // Set the center position for drawing the percentage text:
        XStart+(XEnd-XStart)/2
      }};
    xcb_gcontext_t * GCs = getGCs(&widget);
    widget.X->foreground = GCs[getGC(Percent)];
    widget.X->background = GCs[BATTERY_GC_BACKGROUND];
    drawRectangles(&widget, XStart, XEnd, Percent);
    drawPercent(&widget, Percent);
    xcb_flush(X->xc);
  }
}

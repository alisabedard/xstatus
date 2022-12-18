// Copyright 2017, Alisa Bedard
#ifndef XSTATUS_XSWIDGET_H
#define XSTATUS_XSWIDGET_H
#include "XSXData.h"
#include <stdint.h>
struct XSWidget {
  struct XSXData * X;
  xcb_window_t Window;
  int16_t Geometry[4];
};
#endif//!XSTATUS_XSWIDGET_H

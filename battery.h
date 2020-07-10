// Copyright 2017, Jeffrey E. Bedard
#ifndef XSTATUS_BATTERY_H
#define XSTATUS_BATTERY_H
#include "libjb/JBDim.h"
#include "XSXData.h"
void xstatus_draw_battery(struct XSXData * restrict X,
  const struct JBDim range);
#endif//!XSTATUS_BATTERY_H

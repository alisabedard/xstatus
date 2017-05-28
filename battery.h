// Copyright 2017, Jeffrey E. Bedard
#ifndef XSTATUS_BATTERY_H
#define XSTATUS_BATTERY_H
#include <xcb/xcb.h>
#include "libjb/JBDim.h"
void xstatus_draw_battery(xcb_connection_t * xc, const struct JBDim range);
#endif//!XSTATUS_BATTERY_H

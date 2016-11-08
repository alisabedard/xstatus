// Copyright 2016, Jeffrey E. Bedard
#ifndef XSTATUS_BATTERY_H
#define XSTATUS_BATTERY_H
#include "xdata.h"
#ifdef XSTATUS_USE_BATTERY_BAR
void xstatus_draw_battery(xcb_connection_t * xc, const uint16_t start,
	const uint16_t end);
#endif//XSTATUS_USE_BATTERY_BAR
#endif//!XSTATUS_BATTERY_H

// Copyright 2016, Jeffrey E. Bedard
#ifndef XSTATUS_BATTERY_H
#define XSTATUS_BATTERY_H
#include <xcb/xcb.h>
void xstatus_draw_battery(xcb_connection_t * xc, const uint16_t start,
	const uint16_t end);
#endif//!XSTATUS_BATTERY_H

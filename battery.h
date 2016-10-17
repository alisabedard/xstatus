// Copyright 2016, Jeffrey E. Bedard

#ifndef XSTATUS_BATTERY_H
#define XSTATUS_BATTERY_H

#include "xdata.h"

#ifdef XSTATUS_USE_BATTERY_BAR
void draw_battery(struct XData * restrict X, const uint16_t start,
	const uint16_t end);
#endif//XSTATUS_USE_BATTERY_BAR

#endif//!XSTATUS_BATTERY_H

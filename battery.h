// Copyright 2016, Jeffrey E. Bedard

#ifndef XSTATUS_BATTERY_H
#define XSTATUS_BATTERY_H

#include "xdata.h"

#ifdef USE_BATTERY
void draw_battery(struct XData * restrict X, const uint16_t start,
	const uint16_t end);
#endif//USE_BATTERY

#endif//!XSTATUS_BATTERY_H

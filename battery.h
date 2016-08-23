// Copyright 2016, Jeffrey E. Bedard

#ifndef BATTERY_H
#define BATTERY_H

#include "xdata.h"

#ifdef USE_BATTERY
void draw_battery(struct XData * restrict X, const uint16_t start,
	const uint16_t end);
#endif//USE_BATTERY

#endif//!BATTERY_H

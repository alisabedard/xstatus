// Copyright 2016, Jeffrey E. Bedard

#ifndef XSTATUS_TEMPERATURE_H
#define XSTATUS_TEMPERATURE_H

#ifdef XSTATUS_USE_TEMPERATURE

#include "xdata.h"

#include <stdint.h>

// Returns x offset for next item
uint16_t draw_temp(struct XData * restrict X, const uint16_t offset);

#else//!XSTATUS_USE_TEMPERATURE

#define draw_temp(X, o) o

#endif//XSTATUS_USE_TEMPERATURE

#endif//!XSTATUS_TEMPERATURE_H

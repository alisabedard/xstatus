// Copyright 2016, Jeffrey E. Bedard

#ifndef XS_TEMPERATURE_H
#define XS_TEMPERATURE_H

#ifdef USE_TEMP

#include "xdata.h"

#include <stdint.h>

// Returns x offset for next item
uint16_t draw_temp(XData * restrict X, const uint16_t offset);

#else//!USE_TEMP

#define draw_temp(X, o) o

#endif//USE_TEMP

#endif//!XS_TEMPERATURE_H

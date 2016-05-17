#ifndef XS_TEMPERATURE_H
#define XS_TEMPERATURE_H

#include "xdata.h"
#include <stdint.h>

// Returns x offset for next item
uint16_t draw_temp(XData * restrict X, const uint16_t offset);

#endif//!XS_TEMPERATURE_H

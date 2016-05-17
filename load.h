#ifndef XS_LOAD_H
#define XS_LOAD_H

#include "xdata.h"
#include <stdint.h>

// Returns x offset for next item
uint16_t draw_load(XData * restrict X, const uint16_t offset);

#endif//!XS_LOAD_H

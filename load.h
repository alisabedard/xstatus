// Copyright 2016, Jeffrey E. Bedard

#ifndef XS_LOAD_H
#define XS_LOAD_H

#ifdef USE_LOAD

#include "xdata.h"

// Returns x offset for next item
uint16_t draw_load(XData * restrict X, const uint16_t offset);

#else//!USE_LOAD
#define draw_load(X, o) o
#endif//USE_LOAD

#endif//!XS_LOAD_H

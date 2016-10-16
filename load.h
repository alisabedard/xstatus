// Copyright 2016, Jeffrey E. Bedard

#ifndef XSTATUS_LOAD_H
#define XSTATUS_LOAD_H

#ifdef USE_LOAD

#include "xdata.h"

// Returns x offset for next item
uint16_t draw_load(struct XData * restrict X, const uint16_t offset);

#else//!USE_LOAD
#define draw_load(X, o) o
#endif//USE_LOAD

#endif//!XSTATUS_LOAD_H

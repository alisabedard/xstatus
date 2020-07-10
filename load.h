// Copyright 2017, Jeffrey E. Bedard
#ifndef XSTATUS_LOAD_H
#define XSTATUS_LOAD_H
#include "XSXData.h"
// Returns x offset for next item
uint16_t xstatus_draw_load(struct XSXData * restrict xdata, const uint16_t x)
	__attribute__((nonnull(1),warn_unused_result));
#endif//!XSTATUS_LOAD_H

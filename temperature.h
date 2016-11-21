// Copyright 2016, Jeffrey E. Bedard
#ifndef XSTATUS_TEMPERATURE_H
#define XSTATUS_TEMPERATURE_H
#include <xcb/xcb.h>
// Returns x offset for next item
uint16_t draw_temp(xcb_connection_t * xc, const uint16_t offset);
#endif//!XSTATUS_TEMPERATURE_H

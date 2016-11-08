// Copyright 2016, Jeffrey E. Bedard
#ifndef XSTATUS_CLOCK_H
#define XSTATUS_CLOCK_H
#ifdef XSTATUS_USE_LOCK
#include "xdata.h"
uint16_t xstatus_draw_clock(xcb_connection_t * xc);
#else//!XSTATUS_USE_LOCK
#define xstatus_draw_clock(X)
#endif//XSTATUS_USE_LOCK
#endif//!XSTATUS_CLOCK_H

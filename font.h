// Copyright 2017, Jeffrey E. Bedard
#ifndef XSTATUS_FONT_H
#define XSTATUS_FONT_H
#include <xcb/xcb.h>
#include <stdbool.h>
#include "libjb/JBDim.h"
struct JBDim xstatus_get_font_size(void);
// returns true if successful
bool xstatus_open_font(xcb_connection_t * xc, const char * restrict fn);
#endif//!XSTATUS_FONT_H

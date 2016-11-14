// Copyright 2016, Jeffrey E. Bedard
#ifndef XSTATUS_FONT_H
#define XSTATUS_FONT_H
#include "libjb/JBDim.h"
#include <stdbool.h>
#include <xcb/xcb.h>
xcb_font_t xstatus_get_font(xcb_connection_t * restrict xc);
struct JBDim xstatus_get_font_size(void);
// returns true if successful
bool xstatus_open_font(xcb_connection_t * restrict xc,
	const char * restrict fn);
#endif//!XSTATUS_FONT_H

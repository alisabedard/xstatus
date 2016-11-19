// Copyright 2016, Jeffrey E. Bedard
#include "load.h"
#include "config.h"
#include "font.h"
#include <stdio.h>
#include <stdlib.h>
// Returns x offset for next item
uint16_t xstatus_draw_load(xcb_connection_t * xc, const uint16_t offset)
{
	double l[1];
	getloadavg(l, 1);
	const struct JBDim f = xstatus_get_font_size();
	uint16_t sz=6;
	const int16_t x = offset + XSTATUS_CONST_PAD;
	{ // buf scope
		char buf[sz];
		sz = snprintf(buf, sz, "%.2f", l[0]);
		xcb_image_text_8(xc, sz, xstatus_get_window(xc),
			xstatus_get_gc(xc), x, f.h, buf);
	}
	return x + f.w * sz + XSTATUS_CONST_PAD;
}

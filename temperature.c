// Copyright 2016, Jeffrey E. Bedard
#include "temperature.h"
#include "config.h"
#include "font.h"
#include "util.h"
#include <stdio.h>
static uint16_t get_offset(const uint16_t fw, const uint16_t offset,
	const uint8_t len)
{
	return fw * len + offset + XSTATUS_CONST_WIDE_PAD
		+ XSTATUS_CONST_PAD;
}
// Returns x offset for next item
uint16_t draw_temp(xcb_connection_t * xc, const uint16_t offset)
{
	uint8_t sz = 4;
	const struct JBDim f = xstatus_get_font_size();
	{ // buf scope
		char buf[sz];
		sz = snprintf(buf, sz, "%dC",
			xstatus_system_value(XSTATUS_SYSFILE_TEMPERATURE)
				/ 1000);
		xcb_image_text_8(xc, sz, xstatus_get_window(xc),
			xstatus_get_gc(xc), offset
			+ XSTATUS_CONST_WIDE_PAD, f.h, buf);
	}
	return get_offset(f.w, offset, sz);
}

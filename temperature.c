// Copyright 2016, Jeffrey E. Bedard
#include "temperature.h"
#include "config.h"
#include "font.h"
#include "util.h"
#include <stdio.h>
static uint8_t get_temp(void)
{
	return xstatus_system_value(XSTATUS_SYSFILE_TEMPERATURE) / 1000;
}
static uint8_t format(char * restrict buf, const uint8_t sz)
{
	return snprintf(buf, sz, "%dC", get_temp());
}
// Returns x offset for next item
uint16_t draw_temp(xcb_connection_t * xc, const uint16_t offset)
{
	uint8_t sz = 4;
	const struct JBDim f = xstatus_get_font_size();
	const int16_t x = offset + XSTATUS_CONST_PAD;
	{ // buf scope
		char buf[sz];
		sz = format(buf, sz);
		xcb_image_text_8(xc, sz, xstatus_get_window(xc),
			xstatus_get_gc(xc), x, f.h, buf);
	}
	return x + f.w * sz + XSTATUS_CONST_PAD;
}

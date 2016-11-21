// Copyright 2016, Jeffrey E. Bedard
#include "temperature.h"
#include "config.h"
#include "font.h"
#include "util.h"
#include <stdio.h>
static int32_t get_temp_raw(void)
{
	return xstatus_system_value(XSTATUS_SYSFILE_TEMPERATURE);
}
static uint8_t get_temp(void)
{
	// may only fail once:
	static bool get_temp_failed;
	if (get_temp_failed)
		return 0; // 0 indicates unsupported
	// type must hold at least 100000, signed
	int32_t temp = get_temp_raw();
	if (temp == -1) { // could not open system file
		get_temp_failed = true;
		return 0;
	}
	return temp / 1000;
}
static uint8_t format(char * restrict buf, const uint8_t sz)
{
	const uint8_t temp = get_temp();
	if (temp)
		return snprintf(buf, sz, "%dC", temp);
	else
		return 0;
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
	return x + f.w * sz;
}

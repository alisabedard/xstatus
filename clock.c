// Copyright 2016, Jeffrey E. Bedard
#include "clock.h"
#include "config.h"
#include "font.h"
#include "libjb/xcb.h"
static uint16_t get_offset(xcb_connection_t * restrict xc,
	const uint16_t font_width, const size_t sz)
{
	return xstatus_get_screen(xc)->width_in_pixels
		- font_width * sz;
}
static uint8_t format(char * buf, uint8_t sz)
{
	 return strftime(buf, sz, XSTATUS_TIME_FORMAT,
		 localtime(&(time_t){time(NULL)}));
}
#include <stdio.h>
__attribute__((hot))
uint16_t xstatus_draw_clock(xcb_connection_t * xc)
{
	uint8_t sz = XSTATUS_TIME_BUFFER_SIZE;
	char buf[sz];
	sz = format(buf, sz);
	uint16_t offset;
	fprintf(stderr, "size: %d, string: %s\n", sz, buf);
	{ // f scope
		const struct JBDim f = xstatus_get_font_size();
		offset = get_offset(xc, f.w, sz);
		xcb_image_text_8(xc, sz, xstatus_get_window(xc),
			xstatus_get_gc(xc), offset, f.h, buf);
	}
	return offset;
}

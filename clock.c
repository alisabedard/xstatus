// Copyright 2017, Jeffrey E. Bedard
#include "clock.h"
#include "config.h"
#include "font.h"
static uint8_t format(char * buf, uint8_t sz)
{
	 return strftime(buf, sz, XSTATUS_TIME_FORMAT,
		 localtime(&(time_t){time(NULL)}));
}
__attribute__((hot))
uint16_t xstatus_draw_clock(xcb_connection_t * xc)
{
	uint8_t sz = XSTATUS_TIME_BUFFER_SIZE;
	char buf[sz];
	struct JBDim font_size = xstatus_get_font_size();
	sz = format(buf, sz);
	const uint16_t offset = xstatus_get_screen(xc)->width_in_pixels
		- font_size.width * sz;
	xcb_image_text_8(xc, sz, xstatus_get_window(xc), xstatus_get_gc(xc),
		offset, font_size.height, buf);
	return offset;
}

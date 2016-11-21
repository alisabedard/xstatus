// Copyright 2016, Jeffrey E. Bedard
#include "clock.h"
#include "config.h"
#include "font.h"
#include "libjb/log.h"
#include "libjb/xcb.h"
//#define XSTATUS_DEBUG_CLOCK
#ifndef XSTATUS_DEBUG_CLOCK
#undef LOG
#define LOG(...)
#endif//!XSTATUS_DEBUG_CLOCK
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
static uint16_t draw_for_font_size(xcb_connection_t * restrict xc,
	const struct JBDim font_size, char * buf, const uint8_t sz)
{
	uint16_t offset = get_offset(xc, font_size.w, sz);
	xcb_image_text_8(xc, sz, xstatus_get_window(xc),
		xstatus_get_gc(xc), offset, font_size.h, buf);
	return offset;
}
__attribute__((hot))
uint16_t xstatus_draw_clock(xcb_connection_t * xc)
{
	uint8_t sz = XSTATUS_TIME_BUFFER_SIZE;
	char buf[sz];
	sz = format(buf, sz);
	LOG("size: %d, string: %s\n", sz, buf);
	return draw_for_font_size(xc, xstatus_get_font_size(), buf, sz);
}

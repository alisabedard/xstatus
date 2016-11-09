// Copyright 2016, Jeffrey E. Bedard
#include "util.h"
#include "font.h"
#include "libjb/file.h"
#include "libjb/util.h"
#include "libjb/xcb.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
void xstatus_create_gc(xcb_connection_t * xc, const xcb_gc_t gc,
	const xcb_window_t win, const char * restrict fg,
	const char * restrict bg)
{
	const xcb_colormap_t cm = xstatus_get_colormap(xc);
	xcb_void_cookie_t c = xcb_create_gc_checked(xc, gc, win,
		XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT,
		(uint32_t[]){jb_get_pixel(xc, cm, fg),
		jb_get_pixel(xc, cm, bg), xstatus_get_font(xc)});
	xcb_generic_error_t * e = xcb_request_check(xc, c);
	if (jb_check(!e, "Could not create GC"))
		free(e);
}
#if defined(XSTATUS_USE_BATTERY_BAR) || defined(XSTATUS_USE_TEMPERATURE)
uint32_t xstatus_system_value(const char *filename)
{
#define BUF_SZ 8
	fd_t f = jb_open(filename, O_RDONLY);
	char buf[BUF_SZ] = {};
	read(f, buf, BUF_SZ);
	close(f);
	return atoi(buf);
}
#endif//XSTATUS_USE_BATTERY_BAR||XSTATUS_USE_TEMPERATURE

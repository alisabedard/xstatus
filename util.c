// Copyright 2016, Jeffrey E. Bedard
#include "util.h"
#include "libjb/file.h"
#include "libjb/util.h"
#include "libjb/xcb.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
xcb_gc_t xcbgc(struct XData * restrict X, char * fg, char * bg)
{
	xcb_connection_t * xc = X->xcb;
	const xcb_gc_t gc = xcb_generate_id(xc);
	const xcb_colormap_t cm = X->screen->default_colormap;
	xcb_void_cookie_t c = xcb_create_gc_checked(xc, gc, X->w,
		XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT,
		(uint32_t[]){jb_get_pixel(xc, cm, fg),
		jb_get_pixel(xc, cm, bg), X->font});
	xcb_generic_error_t * e = xcb_request_check(xc, c);
	if (jb_check(!e, "Could not create GC"))
		free(e);
	return gc;
}
#if defined(XSTATUS_USE_BATTERY_BAR) || defined(XSTATUS_USE_TEMPERATURE)
uint32_t sysval(const char *filename)
{
#define BUF_SZ 6
	fd_t f = jb_open(filename, O_RDONLY);
	char buf[BUF_SZ];
	read(f, buf, BUF_SZ);
	close(f);
	return atoi(buf);
}
#endif//XSTATUS_USE_BATTERY_BAR||XSTATUS_USE_TEMPERATURE

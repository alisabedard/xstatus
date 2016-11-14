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
	jb_create_gc(xc, gc, win, fg, bg);
	xcb_change_gc(xc, gc, XCB_GC_FONT, &(uint32_t){xstatus_get_font(xc)});
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

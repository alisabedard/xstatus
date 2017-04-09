// Copyright 2017, Jeffrey E. Bedard
#include "util.h"
#include "font.h"
#include "libjb/file.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
void xstatus_create_gc(xcb_connection_t * xc, const xcb_gcontext_t gc,
	const xcb_window_t win, const char * restrict fg,
	const char * restrict bg)
{
	jb_create_gc(xc, gc, win, fg, bg);
	xcb_change_gc(xc, gc, XCB_GC_FONT,
		&(uint32_t){xstatus_get_font(xc)});
}
// returns -1 on error
int32_t xstatus_system_value(const char * filename)
{
	enum { BUFSZ = 8 };
	char buf[BUFSZ];
	{ // f scope
		fd_t f = jb_open(filename, O_RDONLY);
		if (f == -1)
			return -1;
		read(f, buf, BUFSZ);
		close(f);
	}
	return atoi(buf);
}

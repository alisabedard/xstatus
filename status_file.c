// Copyright 2016, Jeffrey E. Bedard
#include "status_file.h"
#include "config.h"
#include "font.h"
#include "libjb/util.h"
#include "util.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
static ssize_t poll_status_file(const char * restrict filename,
	char * restrict buf)
{
	fd_t fd = open(filename, O_RDONLY|O_CREAT, 0644);
	if (jb_check(fd >= 0, "Could not open status file"))
		return -1;
	ssize_t r = read(fd, buf, XSTATUS_CONST_BUFFER_SIZE);
	jb_check(r != -1, "Could not read status file");
	close(fd);
	return r;
}
static void warn_no_data(const char * restrict fn)
{
	static bool warned;
	if (warned)
		return;
	fprintf(stderr, "No data in status file: %s\n", fn);
	warned = true;
}
// Returns offset for next widget
uint16_t draw_status_file(xcb_connection_t * xc,
	const uint16_t x_offset,
	const char * restrict filename)
{
	char buf[XSTATUS_CONST_BUFFER_SIZE];
	const ssize_t s = poll_status_file(filename, buf) - 1;
	if (s <= 0) { // empty or error
		warn_no_data(filename);
		return x_offset;
	}
	xcb_image_text_8(xc, s, xstatus_get_window(xc),
		xstatus_get_gc(xc), x_offset + XSTATUS_CONST_WIDE_PAD,
		xstatus_get_font_size().h, buf);
	return xstatus_get_font_size().w * s + x_offset
		+ XSTATUS_CONST_WIDE_PAD + XSTATUS_CONST_WIDE_PAD;
}

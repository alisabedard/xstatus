// Copyright 2017, Jeffrey E. Bedard
#include "status_file.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h> // include per man page of fchmod()
#include <unistd.h> // include per man page of read()
#include "XSTextWidget.h"
#include "config.h"
#include "font.h"
#include "libjb/file.h"
#include "xdata.h"
static ssize_t poll_status_file(const char * restrict filename,
	char * restrict buf)
{
	fd_t fd = open(filename, O_RDONLY | O_CREAT);
	fchmod(fd, 0600);
	const ssize_t r = read(fd, buf, XSTATUS_CONST_BUFFER_SIZE);
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
static void draw_text(struct XSTextWidget * restrict w)
{
	xcb_connection_t * xc = w->connection;
	xcb_image_text_8(xc, w->buffer_size, xstatus_get_window(xc),
		xstatus_get_gc(xc), w->offset + (XSTATUS_CONST_PAD << 1),
		w->font_size.height, w->buffer);
}
// Returns offset for next widget
int draw_status_file(xcb_connection_t * xc, int x, const char * filename)
{
	char buf[XSTATUS_CONST_BUFFER_SIZE];
	const ssize_t s = poll_status_file(filename, buf) - 1;
	if (s <= 0) { // empty or error
		warn_no_data(filename);
		return x + XSTATUS_CONST_PAD;
	}
	struct XSTextWidget w = {.connection = xc, .buffer = buf,
		.buffer_size = s, .offset = x,
		.font_size = xstatus_get_font_size()};
	draw_text(&w);
	return w.font_size.width * w.buffer_size + w.offset
		+ (XSTATUS_CONST_PAD << 2);
}

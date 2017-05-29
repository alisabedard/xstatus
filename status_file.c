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
#include "text_widget.h"
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
// Returns offset for next widget
int draw_status_file(xcb_connection_t * xc, short x, const char * filename)
{
	char buf[XSTATUS_CONST_BUFFER_SIZE];
	const uint8_t pad = XSTATUS_CONST_PAD << 1;
	struct XSTextWidget w = {.connection = xc, .buffer = buf,
		.buffer_size = poll_status_file(filename, buf) - 1,
		.offset = x + pad};
	const bool valid = w.buffer_size > 0;
	if (valid)
		xstatus_draw_text_widget(&w);
	else
		warn_no_data(filename);
	return valid ? xstatus_get_font_size().width * w.buffer_size
		+ w.offset + pad : w.offset;
}

// Copyright 2017, Jeffrey E. Bedard
#include "status_file.h"
#include <sys/stat.h> // include per man page of fchmod()
#include <unistd.h> // include per man page of read()
#include "XSTextWidget.h"
#include "libjb/file.h"
#include "config.h"
#include "text_widget.h"
static ssize_t poll_status_file(const char * restrict filename,
	char * restrict buf)
{
	fd_t fd = open(filename, O_RDONLY | O_CREAT);
	fchmod(fd, 0600);
	const ssize_t r = read(fd, buf, XSTATUS_CONST_BUFFER_SIZE);
	close(fd);
	return r;
}
// ret is a pass-through return value to allow chained calling
static short warn_no_data(const char * restrict fn, short ret)
{
	static unsigned char warned;
	if (warned > 0) {
		char msg[] = "No data in status file ";
		write(2, msg, sizeof(msg));
		size_t len = 0;
		while (fn[++len]);
		write(2, fn, len);
		write(2, "\n", 1);
		++warned;
	}
	return ret;
}
// Returns offset for next widget
short draw_status_file(void * xc, short x, const char * filename)
{
	char buf[XSTATUS_CONST_BUFFER_SIZE];
	enum { PAD = XSTATUS_CONST_PAD << 1 };
	struct XSTextWidget w = {.connection = xc, .buffer = buf,
		.buffer_size = poll_status_file(filename, buf) - 1,
		.offset = x + PAD};
	return w.buffer_size > 0 ? xstatus_draw_text_widget(&w) :
		warn_no_data(filename, w.offset);
}

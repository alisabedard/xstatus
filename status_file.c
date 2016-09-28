#include "status_file.h"

#include "config.h"
#include "libjb/util.h"
#include "util.h"

#include <fcntl.h>
#include <unistd.h>

static ssize_t poll_status_file(const char * restrict filename,
	char * restrict buf)
{
	int fd = open(filename, O_RDONLY|O_CREAT);
	ssize_t r = read(fd, buf, XS_BUF_SZ);
	jb_check(r != -1, "Could not read status file");
	jb_close(fd);
	return r;
}

// Returns offset for next widget
uint16_t draw_status_file(struct XData * restrict X,
	const uint16_t x_offset,
	const char * restrict filename)
{
	char buf[XS_BUF_SZ];
	const ssize_t s = poll_status_file(filename, buf) - 1;
	if (s <= 0) // empty or error
		return x_offset;
	xcb_image_text_8(X->xcb, s, X->w, X->gc, x_offset + XS_WPAD,
		X->font_size.h, buf);
	return X->font_size.w * s + x_offset + XS_WPAD + XS_WPAD;
}


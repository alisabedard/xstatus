#include "status_file.h"

#include "config.h"
#include "libjb/util.h"
#include "util.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

static ssize_t poll_status_file(const char * restrict filename,
	char * restrict buf)
{
	fd_t fd = open(filename, O_RDONLY|O_CREAT, 0644);
	errno = 0;
	if (jb_check(fd >= 0, "Could not open status file"))
		return -1;
	errno = 0;
	ssize_t r = read(fd, buf, XS_BUF_SZ);
	jb_check(r != -1, "Could not read status file");
	errno = 0;
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
	if (s <= 0) { // empty or error
		static bool been_warned;
		if (been_warned)
			goto skip_warning;
		const char msg[] = "No data in status file: ";
		write(2, msg, sizeof(msg));
		size_t l = 0;
		while(filename[++l]);
		write(2, filename, l);
		write(2, "\n", 1);
		been_warned = true;
skip_warning:
		return x_offset;
	}
	xcb_image_text_8(X->xcb, s, X->w, X->gc, x_offset + XS_WPAD,
		X->font_size.h, buf);
	return X->font_size.w * s + x_offset + XS_WPAD + XS_WPAD;
}


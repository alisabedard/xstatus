#include "status_file.h"

#include "config.h"
#include "util.h"

enum { STATUS_BUF_SZ = 80 };

static size_t poll_status_file(const char * restrict filename,
	char *buf)
{
	FILE * restrict f = fopen(filename, "a+");
	size_t s = fread(buf, 1, STATUS_BUF_SZ, f);
	fclose(f);
	return s;
}

// Returns offset for next widget
uint16_t draw_status_file(XData * restrict X,
	const uint16_t x_offset,
	const char * restrict filename)
{
	char buf[STATUS_BUF_SZ];
	const size_t s = poll_status_file(filename, buf) - 1;
	xcb_image_text_8(X->xcb, s, X->w, X->gc, x_offset
		+ BIGPAD, X->font_height, buf);
	return X->font_width * s + x_offset + BIGPAD;
}

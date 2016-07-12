#include "load.h"

#include "config.h"
#include "util.h"

#include <stdlib.h>
#include <string.h>

// Returns x offset for next item
uint16_t draw_load(XData * restrict X, const uint16_t offset)
{
	double l[1];
	getloadavg(l, 1);
	uint16_t sz=6;
	char buf[sz];
	sz = snprintf(buf, sz, "%.2f", l[0]);
	xcb_image_text_8(X->xcb, sz, X->w, X->gc,
		offset + BIGPAD, X->font_height, buf);
	return X->font_width * sz + offset + BIGPAD;
}


// Copyright 2016, Jeffrey E. Bedard
#include "load.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
// Returns x offset for next item
uint16_t draw_load(struct XData * restrict X, const uint16_t offset)
{
	double l[1];
	getloadavg(l, 1);
	uint16_t sz=6;
	char buf[sz];
	sz = snprintf(buf, sz, "%.2f", l[0]);
	const struct JBDim f = X->font_size;
	xcb_image_text_8(X->xcb, sz, X->w, X->gc,
		offset + XSTATUS_CONST_PAD, f.h, buf);
	return f.w * sz + offset + XSTATUS_CONST_WIDE_PAD;
}

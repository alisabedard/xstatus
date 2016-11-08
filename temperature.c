// Copyright 2016, Jeffrey E. Bedard
#include "temperature.h"
#include "config.h"
#include "font.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
// Returns x offset for next item
uint16_t draw_temp(struct XData * restrict X, const uint16_t offset)
{
	const uint8_t v
		= xstatus_system_value(XSTATUS_SYSFILE_TEMPERATURE)/1000;
	uint8_t sz = 4;
	char buf[sz];
	sz = snprintf(buf, sz, "%dC", v);
	const struct JBDim f = xstatus_get_font_size();
	xcb_image_text_8(X->xcb, sz, X->w, X->gc, offset
		+ XSTATUS_CONST_WIDE_PAD, f.h, buf);
	return f.w * strlen(buf) + offset + XSTATUS_CONST_WIDE_PAD
		+ XSTATUS_CONST_PAD;
}

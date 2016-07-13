// Copyright 2016, Jeffrey E. Bedard

#include "temperature.h"

#include "config.h"
#include "util.h"

#include <stdio.h>
#include <string.h>

// Returns x offset for next item
uint16_t draw_temp(XData * restrict X, const uint16_t offset)
{
	const uint8_t v = sysval(TEMPSYSFILE)/1000;
	uint8_t sz = 4;
	char buf[sz];
	sz = snprintf(buf, 4, "%dC", v);
	xcb_image_text_8(X->xcb, sz, X->w, X->gc, offset
		+ (PAD<<1), X->font_height, buf);
	return X->font_width * strlen(buf) + offset + (PAD<<1)
		+ (PAD<<1);
}


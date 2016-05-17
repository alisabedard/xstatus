#include "temperature.h"

#include "config.h"
#include "util.h"

#include <string.h>

// Returns x offset for next item
uint16_t draw_temp(XData * restrict X, const uint16_t offset)
{
	const uint8_t v = sysval(TEMPSYSFILE)/1000;
	char buf[4];
	snprintf(buf, 4, "%dC", v);
	XDrawString(X->d, X->w, X->gc, offset+PAD+1, font_y(X->font),
		buf, strlen(buf));
	return XTextWidth(X->font, buf, strlen(buf)) + offset + (PAD<<1);
}


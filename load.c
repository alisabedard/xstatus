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
	static const uint16_t sz=6;
	char buf[sz];
	snprintf(buf, sz, "%.2f", l[0]);
	XDrawString(X->d, X->w, X->gc, offset+PAD+1, font_y(X->font),
		buf, strlen(buf));
	return XTextWidth(X->font, buf, strlen(buf)) + offset + PAD;
}


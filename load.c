// Copyright 2017, Jeffrey E. Bedard
#include "load.h"
#include <stdio.h>
#include <stdlib.h>
#include "XSTextWidget.h"
#include "text_widget.h"
__attribute__((nonnull(1)))
static uint8_t format(char * restrict buf, const uint8_t sz)
{
	double l;
	getloadavg(&l, 1);
	return snprintf(buf, sz, "%.2f", l);
}
// Returns x offset for next item
uint16_t xstatus_draw_load(struct XSXData * restrict xdata, const uint16_t x)
{
	enum { BUFSZ = 6 };
	char buf[BUFSZ];
	struct XSTextWidget w = {xdata, buf, format(buf, BUFSZ), x};
	return xstatus_draw_text_widget(&w);
}

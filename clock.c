// Copyright 2017, Jeffrey E. Bedard
#include "clock.h"
#include "XSTextWidget.h"
#include "config.h"
#include "font.h"
#include "text_widget.h"
static uint8_t format(char * buf, uint8_t sz)
{
	 return strftime(buf, sz, XSTATUS_TIME_FORMAT,
		 localtime(&(time_t){time(NULL)}));
}
__attribute__((hot))
uint16_t xstatus_draw_clock(struct XSXData * x)
{
	char buf[XSTATUS_TIME_BUFFER_SIZE];
	struct XSTextWidget w = {x->xc, buf,
          format(buf, XSTATUS_TIME_BUFFER_SIZE), x->screen->width_in_pixels};
	w.offset -= xstatus_get_font_size().width * w.buffer_size;
	xstatus_draw_text_widget(&w);
	return w.offset;
}

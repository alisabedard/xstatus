// Copyright 2017, Alisa Bedard
#include "text_widget.h"
#include "XSTextWidget.h"
#include "config.h"
#include "font.h"
#include "XSXData.h"
short xstatus_draw_text_widget(struct XSTextWidget * widget)
{
	xcb_connection_t * xc = widget->x->xc;
	struct JBDim font_size = xstatus_get_font_size();
	xcb_image_text_8(xc, widget->buffer_size, widget->x->window,
          widget->x->gc, widget->offset, font_size.height, widget->buffer);
	return widget->offset + font_size.width * widget->buffer_size
		+ XSTATUS_CONST_PAD;
}

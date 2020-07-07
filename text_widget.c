// Copyright 2017, Jeffrey E. Bedard
#include "text_widget.h"
#include "XSTextWidget.h"
#include "config.h"
#include "font.h"
#include "XSXData.h"
short xstatus_draw_text_widget(struct XSTextWidget * widget)
{
	xcb_connection_t * xc = widget->connection;
	struct JBDim font_size = xstatus_get_font_size();
	xcb_image_text_8(xc, widget->buffer_size, xstatus_get_window(xc),
		xstatus_get_gc(xc), widget->offset, font_size.height,
		widget->buffer);
	return widget->offset + font_size.width * widget->buffer_size
		+ XSTATUS_CONST_PAD;
}

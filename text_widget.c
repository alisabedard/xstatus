// Copyright 2017, Jeffrey E. Bedard
#include "text_widget.h"
#include "XSTextWidget.h"
#include "font.h"
#include "xdata.h"
void xstatus_draw_text_widget(struct XSTextWidget * widget)
{
	xcb_connection_t * xc = widget->connection;
	xcb_image_text_8(xc, widget->buffer_size, xstatus_get_window(xc),
		xstatus_get_gc(xc), widget->offset,
		xstatus_get_font_size().height, widget->buffer);
}

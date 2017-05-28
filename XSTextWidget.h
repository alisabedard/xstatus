// Copyright 2017, Jeffrey E. Bedard
#ifndef XSTATUS_XSTEXTWIDGET_H
#define XSTATUS_XSTEXTWIDGET_H
#include <xcb/xcb.h>
#include "libjb/JBDim.h"
struct XSTextWidget {
	xcb_connection_t * connection;
	char * buffer;
	uint16_t buffer_size, offset;
	struct JBDim font_size;
};
#endif//!XSTATUS_XSTEXTWIDGET_H


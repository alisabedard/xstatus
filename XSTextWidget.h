// Copyright 2017, Jeffrey E. Bedard
#ifndef XSTATUS_XSTEXTWIDGET_H
#define XSTATUS_XSTEXTWIDGET_H
struct XSTextWidget {
	void * connection;
	char * buffer;
	short buffer_size, offset;
};
#endif//!XSTATUS_XSTEXTWIDGET_H

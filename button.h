// Copyright 2017, Jeffrey E. Bedard
#ifndef XSTATUS_BUTTON_H
#define XSTATUS_BUTTON_H
#include "XSWidget.h"
#include "libjb/JBDim.h"
struct XSButton {
	struct XSButton * next;
	struct XSWidget widget;
	void (*cb)(struct XSButton *);
	void (*draw) (struct XSButton *);
	void (*enter) (struct XSButton *);
	void (*leave) (struct XSButton *);
	void * cb_data;
	char * label;
	unsigned short length;
	struct JBDim font_size;
	xcb_gcontext_t gc;
};
struct XSButton * xstatus_create_button(xcb_connection_t * restrict xc,
	const int16_t x, char * label);
#endif//XSTATUS_BUTTON_H

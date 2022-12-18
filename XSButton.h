// Copyright 2020, Alisa Bedard
#ifndef XSTATUS_XSBUTTON_H
#define XSTATUS_XSBUTTON_H
#include "libjb/JBDim.h"
#include "XSWidget.h"
#include <stdint.h>
struct XSButton {
	struct XSButton * next;
	struct XSWidget widget;
	void (*cb)(struct XSButton *);
	void (*draw) (struct XSButton *);
	void (*enter) (struct XSButton *);
	void (*leave) (struct XSButton *);
	void * cb_data;
	char * label;
	uint16_t length;
	struct JBDim font_size;
};
#endif//!XSTATUS_XSBUTTON_H

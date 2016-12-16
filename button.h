// Copyright 2016, Jeffrey E. Bedard
#ifndef XSTATUS_BUTTON_H
#define XSTATUS_BUTTON_H
#include <xcb/xcb.h>
struct XSButton {
	struct XSButton * next;
	void (*cb)(struct XSButton *);
	void (*draw) (struct XSButton *);
	void (*enter) (struct XSButton *);
	void (*leave) (struct XSButton *);
	void * cb_data;
	char * label;
	xcb_connection_t * xc;
	xcb_window_t window;
	int16_t x;
	uint16_t width;
};
struct XSButton * xstatus_create_button(xcb_connection_t * restrict xc,
	const int16_t x, char * label);
#endif//XSTATUS_BUTTON_H

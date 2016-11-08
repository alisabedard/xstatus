#ifndef XSTATUS_BUTTON_H
#define XSTATUS_BUTTON_H
#include <stdint.h>
#include "widget.h"
struct XStatusButton {
	char * label;
	void (*cb) (struct XStatusButton *);
	void (*draw) (struct XStatusButton *);
	void *cb_data;
	xcb_connection_t * xc;
	struct XStatusButton * next; // optional linked list
	struct XStatusWidget widget;
};
struct XStatusButton * xstatus_get_button(xcb_connection_t * xc,
	xcb_rectangle_t * restrict g,
	char * restrict label, void (*cb)(struct XStatusButton *),
	void *cb_data);
#endif//XSTATUS_BUTTON_H

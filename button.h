#ifndef XSTATUS_BUTTON_H
#define XSTATUS_BUTTON_H
#include <stdint.h>
#include "widget.h"
struct XStatusButton {
	char * label;
	void (*cb) (struct XStatusButton *);
	void (*draw) (struct XStatusButton *);
	void *cb_data;
	struct XStatusButton * next; // optional linked list
	struct XStatusWidget widget;
};
struct XStatusButton * xstatus_get_button(struct XData * restrict X,
	xcb_rectangle_t * restrict g,
	char * restrict label, void (*cb)(struct XStatusButton *),
	void *cb_data);
#endif//XSTATUS_BUTTON_H

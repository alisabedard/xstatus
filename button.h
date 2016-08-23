#ifndef _BUTTON_H
#define _BUTTON_H

#include <stdint.h>
#include "widget.h"

struct Button {
	char * label;
	void (*cb) (struct Button *);
	void (*draw) (struct Button *);
	void *cb_data;
	struct Button * next; // optional linked list
	struct Widget widget;
};

struct Button * get_button(struct XData * restrict X,
	xcb_rectangle_t * restrict g,
	char * restrict label, void (*cb)(struct Button *),
	void *cb_data);

#endif//_BUTTON_H

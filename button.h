#ifndef _BUTTON_H
#define _BUTTON_H

#include <stdint.h>
#include "widget.h"

typedef struct Button {
	char * label;
	void (*cb) (struct Button *);
	void (*draw) (struct Button *);
	void *cb_data;
	struct Button * next; // optional linked list
	Widget widget;
} Button;

Button * get_button(XData * restrict X,
	xcb_rectangle_t * restrict g,
	char * restrict label, void (*cb)(Button *),
	void *cb_data);

#endif//_BUTTON_H

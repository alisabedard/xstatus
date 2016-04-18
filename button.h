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

void setup_Button(Button * restrict b, XData * restrict X, 
	XRectangle * restrict g, char * restrict label, 
	void (*cb)(Button *), void *cb_data);

Button * new_Button(XData * restrict X, XRectangle * restrict g,
	char * restrict label, void (*cb)(Button *), void *cb_data);

#endif//_BUTTON_H

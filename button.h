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

Button * new_Button(Display * restrict d, const Window parent, const GC gc,
	const uint16_t x, char * restrict label, void (*cb)(Button *),
	void * restrict cb_data);

#endif//_BUTTON_H

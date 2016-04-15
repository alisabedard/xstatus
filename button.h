#ifndef _BUTTON_H
#define _BUTTON_H

#include <stdint.h>
#include "widget.h"

typedef struct Button {
	char * label;
	void (*cb) (struct Button *);
	void *cb_data;
	struct Button * next; // optional linked list
	Widget widget;
} Button;

Button * new_Button(Display * restrict d, const Window parent, const GC gc,
	char * restrict label, const uint16_t x, void (*cb)(Button *),
	void * restrict cb_data);

void delete_Button(Button * b);

Button * cmd_Button(Display * restrict d, const Window w, const GC gc,
	const uint16_t x, char * restrict label, char * restrict cmd);

void draw_Button(Button * restrict b);

#endif//_BUTTON_H

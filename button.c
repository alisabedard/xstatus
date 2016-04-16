#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "button.h"
#include "config.h"
#include "log.h"
#include "util.h"
#include "xstatus.h"

void draw_Button(Button * restrict b)
{
	assert(b->widget.d);
	assert(b->widget.window);
	XClearWindow(b->widget.d, b->widget.window);
	XDrawString(b->widget.d, b->widget.window, b->widget.gc, PAD,
		font_y(), b->label, strlen(b->label));
}

Button * new_Button(Display * restrict d, const Window parent, const GC gc,
	const uint16_t x, char * restrict label, void (*cb)(Button *),
	void *cb_data)
{
	Button * restrict b = malloc(sizeof(Button));
	b->label=label;
	b->cb=cb;
	b->draw=&draw_Button;
	b->cb_data=cb_data;
	b->next=NULL;
	XRectangle g = { x, 0, string_width(strlen(label)), HEIGHT-2*BORDER};
	create_widget(&b->widget, d, parent, &g, BORDER,
		pixel(d, BUTTON_BORDER), pixel(d, BUTTON_BG), gc);
	XSelectInput(d, b->widget.window, ExposureMask | ButtonPressMask);
	draw_Button(b);
	return b;
}


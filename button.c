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

uint16_t xstatus_row_x;

static void system_cb(Button * b)
{
	const char *cmd = b->cb_data;
	if (system(cmd))
		WARN("Cannot execute %s\n", cmd);
}

Button * new_Button(Display * restrict d, const Window parent, const GC gc,
	char * restrict label, void (*cb)(Button *), void *cb_data)
{
	Button * restrict b = malloc(sizeof(struct _Button));
	b->label=label;
	b->cb=cb;
	b->cb_data=cb_data;
	XRectangle g = { xstatus_row_x, 0, 
		string_width(strlen(label)), HEIGHT-2*BORDER};
	create_widget(&b->widget, d, parent, &g, BORDER,
		pixel(d, BUTTON_BORDER), pixel(d, BUTTON_BG), gc);
	xstatus_row_x += g.width + BUTTON_SPACE;
	XSelectInput(d, b->widget.window, ExposureMask | ButtonPressMask);
	draw_Button(b);
	return b;
}

Button * cmd_Button(Display * restrict d, const Window w, const GC gc,
	char * restrict label, char * restrict cmd)
{
	return new_Button(d, w, gc, label,
		&system_cb, cmd);
}

void draw_Button(Button * restrict b)
{
	assert(b->widget.d);
	assert(b->widget.window);
	XClearWindow(b->widget.d, b->widget.window);
	XDrawString(b->widget.d, b->widget.window, b->widget.gc, PAD,
		font_y(), b->label, strlen(b->label));
}


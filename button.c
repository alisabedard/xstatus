#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "button.h"
#include "config.h"
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
	b->d=d;
	b->parent=parent;
	b->gc=gc;
	b->g.x=xstatus_row_x;
	b->g.y=0;
	b->g.width=string_width(strlen(label));
	b->g.height= HEIGHT - 2 * BORDER; 
	b->w = XCreateSimpleWindow(d, parent, xstatus_row_x, b->g.y, b->g.width, 
		HEIGHT - 2 * BORDER, BORDER, pixel(d, BUTTON_BORDER),
		pixel(d, BUTTON_BG));
	xstatus_row_x += b->g.width + BUTTON_SPACE;
	XSelectInput(d, b->w, ExposureMask | ButtonPressMask);
	XMapWindow(d, b->w);
	return b;
}

Button * cmd_Button(Display * restrict d, const Window w, const GC gc,
	char * restrict label, char * restrict cmd)
{
	return new_Button(d, w, gc, label, &system_cb, cmd);
}

void draw_Button(Button * restrict b)
{
	XClearWindow(b->d, b->w);
	XDrawString(b->d, b->w, b->gc, PAD, font_y(), b->label, 
		strlen(b->label));
}


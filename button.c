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

static void draw_Button(Button * restrict b)
{
	const Widget * restrict w = &b->widget;

	XClearWindow(w->X->d, w->window);
	XDrawString(w->X->d, w->window, w->X->gc, PAD,
		font_y(w->X->font),
		b->label, strlen(b->label));
}

void setup_Button(Button * restrict b, XData * restrict X,
	xcb_rectangle_t * restrict g, char * restrict label,
	void (*cb)(Button *), void *cb_data)
{
	b->label=label;
	b->cb=cb;
	b->draw=&draw_Button;
	b->cb_data=cb_data;
	b->next=NULL;
	Widget * w = &b->widget;
	setup_Widget(&b->widget, X, g, pixel(X, BUTTON_BG));
	memcpy(&w->geometry, g, sizeof(XRectangle));
	XSelectInput(X->d, w->window, ExposureMask
		| ButtonPressMask);
	draw_Button(b);
}

Button * new_Button(XData * restrict X,
	xcb_rectangle_t * restrict geometry,
	char * restrict label, void (*cb)(Button *),
	void *cb_data)
{
	Button * restrict b = malloc(sizeof(Button));
	setup_Button(b, X, geometry, label, cb, cb_data);
	return b;
}


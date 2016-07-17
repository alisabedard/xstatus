// Copyright 2016, Jeffrey E. Bedard

#include "button.h"

#include "config.h"
#include "libjb/xcb.h"

#include <stdlib.h>
#include <string.h>

static void draw_Button(Button * restrict b)
{
	const Widget * restrict w = &b->widget;
	xcb_image_text_8(w->X->xcb, strlen(b->label), w->window,
		w->X->gc, PAD, w->X->font_height, b->label);
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
	const xcb_colormap_t cm = X->screen->default_colormap;
	setup_Widget(&b->widget, X, g, jb_get_pixel(X->xcb, cm, BUTTON_BG),
		XCB_EVENT_MASK_EXPOSURE
		| XCB_EVENT_MASK_BUTTON_PRESS);
	memcpy(&w->geometry, g, sizeof(xcb_rectangle_t));
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


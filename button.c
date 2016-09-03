// Copyright 2016, Jeffrey E. Bedard

#include "button.h"

#include "config.h"
#include "libjb/xcb.h"

#include <stdlib.h>
#include <string.h>

static void draw(struct Button * restrict b)
{
	const struct Widget * restrict w = &b->widget;
	xcb_image_text_8(w->X->xcb, strlen(b->label), w->window,
		w->X->gc, PAD, w->X->font_size.height, b->label);
}

static void setup(struct Button * restrict b, struct XData * restrict X,
	xcb_rectangle_t * restrict g, char * restrict label,
	void (*cb)(struct Button *), void *cb_data)
{
	b->label = label;
	b->draw = draw;
	b->cb = cb;
	b->cb_data = cb_data;
	b->next = NULL;
	struct Widget * w = &b->widget;
	const xcb_colormap_t cm = X->screen->default_colormap;
	setup_Widget(&b->widget, X, g, jb_get_pixel(X->xcb, cm, BUTTON_BG),
		XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS);
	memcpy(&w->geometry, g, sizeof(xcb_rectangle_t));
	draw(b);
}

struct Button * get_button(struct XData * restrict X,
	xcb_rectangle_t * restrict geometry,
	char * restrict label, void (*cb)(struct Button *),
	void *cb_data)
{
	struct Button * restrict b = malloc(sizeof(struct Button));
	setup(b, X, geometry, label, cb, cb_data);
	return b;
}


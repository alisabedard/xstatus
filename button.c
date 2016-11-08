// Copyright 2016, Jeffrey E. Bedard
#include "button.h"
#include "config.h"
#include "font.h"
#include "libjb/xcb.h"
#include <stdlib.h>
#include <string.h>
static void draw(struct XStatusButton * restrict b)
{
	const struct XStatusWidget * restrict w = &b->widget;
	xcb_image_text_8(w->X->xcb, strlen(b->label), w->window,
		w->X->gc, XSTATUS_CONST_PAD, xstatus_get_font_size().height,
		b->label);
}
static void setup(struct XStatusButton * restrict b, struct XData * restrict X,
	xcb_rectangle_t * restrict g, char * restrict label,
	void (*cb)(struct XStatusButton *), void *cb_data)
{
	b->label = label;
	b->draw = draw;
	b->cb = cb;
	b->cb_data = cb_data;
	b->next = NULL;
	struct XStatusWidget * w = &b->widget;
	const xcb_colormap_t cm = X->screen->default_colormap;
	xstatus_get_widget(&b->widget, X, g,
		jb_get_pixel(X->xcb, cm, XSTATUS_BUTTON_BG),
		XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS);
	memcpy(&w->geometry, g, sizeof(xcb_rectangle_t));
	draw(b);
}
struct XStatusButton * xstatus_get_button(struct XData * restrict X,
	xcb_rectangle_t * restrict geometry,
	char * restrict label, void (*cb)(struct XStatusButton *),
	void *cb_data)
{
	struct XStatusButton * restrict b
		= malloc(sizeof(struct XStatusButton));
	setup(b, X, geometry, label, cb, cb_data);
	return b;
}

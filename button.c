// Copyright 2016, Jeffrey E. Bedard
#include "button.h"
#include "config.h"
#include "font.h"
#include "libjb/xcb.h"
#include "xdata.h"
#include <stdlib.h>
#include <string.h>
static void draw(struct XStatusButton * restrict b)
{
	const struct XStatusWidget * restrict w = &b->widget;
	xcb_connection_t * xc = b->xc;
	xcb_image_text_8(xc, strlen(b->label), w->window,
		xstatus_get_button_gc(xc), XSTATUS_CONST_PAD,
		xstatus_get_font_size().height, b->label);
}
static void setup(struct XStatusButton * restrict b, xcb_connection_t * xc,
	xcb_rectangle_t * restrict g, char * restrict label,
	void (*cb)(struct XStatusButton *), void *cb_data)
{
	b->xc = xc;
	b->label = label;
	b->draw = draw;
	b->cb = cb;
	b->cb_data = cb_data;
	b->next = NULL;
	struct XStatusWidget * w = &b->widget;
	xstatus_setup_widget(xc, &b->widget, g,
		jb_get_pixel(xc, xstatus_get_colormap(xc),
		XSTATUS_BUTTON_BG),
		XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS);
	w->geometry = *g;
	draw(b);
}
struct XStatusButton * xstatus_get_button(xcb_connection_t * xc,
	xcb_rectangle_t * restrict geometry,
	char * restrict label, void (*cb)(struct XStatusButton *),
	void *cb_data)
{
	struct XStatusButton * restrict b
		= malloc(sizeof(struct XStatusButton));
	setup(b, xc, geometry, label, cb, cb_data);
	return b;
}

// Copyright 2016, Jeffrey E. Bedard
#include "button.h"
#include "config.h"
#include "font.h"
#include "libjb/xcb.h"
#include "xdata.h"
#include <stdlib.h>
#include <string.h>
static void xstatus_draw_button(struct XSButton * restrict b)
{
	xcb_image_text_8(b->xc, strlen(b->label), b->window,
		xstatus_get_button_gc(b->xc), XSTATUS_CONST_PAD,
		xstatus_get_font_size().height, b->label);
}
struct XSButton * xstatus_create_button(xcb_connection_t * restrict xc,
	const int16_t x, char * label)
{
	struct XSButton * b = calloc(1, sizeof(struct XSButton));
	b->window = xcb_generate_id(xc);
	b->draw = xstatus_draw_button;
	b->label = label;
	b->xc = xc;
	const struct JBDim f = xstatus_get_font_size();
	xcb_create_window(xc, XCB_COPY_FROM_PARENT, b->window,
		xstatus_get_window(xc), b->x = x, 0,
		b->width = f.w * strlen(label) + f.w,
		f.h + XSTATUS_CONST_PAD, 0, 0, 0, XCB_CW_BACK_PIXEL
		| XCB_CW_EVENT_MASK, (uint32_t[]){jb_get_pixel(xc,
		xstatus_get_colormap(xc), XSTATUS_BUTTON_BG),
		XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS});
	xcb_map_window(xc, b->window);
	xstatus_draw_button(b);
	return b;
}

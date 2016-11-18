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
static pixel_t get_bg(xcb_connection_t * restrict xc)
{
	static pixel_t p; // cache the result
	return p ? p : (p = jb_get_pixel(xc, xstatus_get_colormap(xc),
		XSTATUS_BUTTON_BG));
}
static inline uint16_t get_width(uint16_t fw, const char * label)
{
	return fw * strlen(label) + fw;
}
static inline uint16_t get_height(uint16_t fh)
{
	return fh + XSTATUS_CONST_PAD;
}
static void create_window(struct XSButton * b)
{
	const struct JBDim f = xstatus_get_font_size();
	b->width = get_width(f.w, b->label);
	const uint32_t vm = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	const uint32_t em = XCB_EVENT_MASK_EXPOSURE
		| XCB_EVENT_MASK_BUTTON_PRESS;
	xcb_create_window(b->xc, 0, b->window, xstatus_get_window(b->xc),
		b->x, 0, b->width, get_height(f.h), 0, 0, 0, vm,
		(uint32_t[]){get_bg(b->xc), em});
	xcb_map_window(b->xc, b->window);
}
struct XSButton * xstatus_create_button(xcb_connection_t * restrict xc,
	const int16_t x, char * label)
{
	struct XSButton * b = calloc(1, sizeof(struct XSButton));
	b->window = xcb_generate_id(b->xc = xc);
	b->draw = xstatus_draw_button;
	b->label = label;
	b->x = x;
	create_window(b);
	xstatus_draw_button(b);
	return b;
}

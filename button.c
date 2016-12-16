// Copyright 2016, Jeffrey E. Bedard
#include "button.h"
#include "config.h"
#include "font.h"
#include "libjb/xcb.h"
#include "xdata.h"
#include <stdlib.h>
#include <string.h>
static void draw(struct XSButton * restrict b)
{
	xcb_connection_t * xc = b->xc;
	xcb_image_text_8(xc, strlen(b->label), b->window,
		xstatus_get_button_gc(xc), XSTATUS_CONST_PAD,
		xstatus_get_font_size().height, b->label);
}
static void invert(struct XSButton * restrict b)
{
	xcb_connection_t * xc = b->xc;
	const xcb_window_t w = b->window;
	const xcb_gcontext_t gc = xstatus_get_invert_gc(xc);
	const struct JBDim f = xstatus_get_font_size();
	xcb_poly_fill_rectangle(xc, w, gc, 1, &(xcb_rectangle_t){0, 0,
		b->width, f.h});
	xcb_flush(xc);
}
static pixel_t get_bg(xcb_connection_t * restrict xc)
{
	static pixel_t p; // cache the result
	return p ? p : (p = jb_get_pixel(xc, xstatus_get_colormap(xc),
		XSTATUS_BUTTON_BG));
}
static inline uint16_t get_width(uint8_t fw, const char * label)
{
	return fw * strlen(label) + fw;
}
static inline uint8_t get_height(uint8_t fh)
{
	return fh + (XSTATUS_CONST_PAD >> 1);
}
static xcb_rectangle_t get_geometry(struct XSButton * b)
{
	const struct JBDim f = xstatus_get_font_size();
	return (xcb_rectangle_t){.x = b->x,
		.width = b->width = get_width(f.w, b->label),
		.height = get_height(f.h)};
}
static void create_window(struct XSButton * b)
{
	const xcb_window_t w = b->window;
	xcb_connection_t * restrict xc = b->xc;
	{ // g scope, vm scope, em scope
		enum {
			VM = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK,
			EM = XCB_EVENT_MASK_EXPOSURE
				| XCB_EVENT_MASK_BUTTON_PRESS
				| XCB_EVENT_MASK_ENTER_WINDOW
				| XCB_EVENT_MASK_LEAVE_WINDOW,
			CFP = XCB_COPY_FROM_PARENT,
			BORDER = 0
		};
		const xcb_rectangle_t g = get_geometry(b);
		xcb_create_window(xc, CFP, w, xstatus_get_window(xc),
			g.x, g.y, g.width, g.height, BORDER,
			CFP, CFP, VM, (uint32_t[]){get_bg(xc), EM});
	}
	xcb_map_window(xc, w);
}
struct XSButton * xstatus_create_button(xcb_connection_t * restrict xc,
	const int16_t x, char * label)
{
	struct XSButton * b = calloc(1, sizeof(struct XSButton));
	b->window = xcb_generate_id(b->xc = xc);
	b->label = label;
	b->draw = draw;
	b->enter = invert;
	b->leave = invert;
	b->x = x;
	create_window(b);
	draw(b);
	return b;
}

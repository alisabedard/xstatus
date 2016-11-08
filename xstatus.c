// Copyright 2016, Jeffrey E. Bedard
#include "xstatus.h"
#include "battery.h"
#include "button.h"
#include "clock.h"
#include "config.h"
#include "font.h"
#include "libjb/class.h"
#include "libjb/log.h"
#include "libjb/xcb.h"
#include "load.h"
#include "status_file.h"
#include "temperature.h"
#include "util.h"
#include <string.h>
static struct XStatusButton * xstatus_head_button;
static char * xstatus_filename;
static void create_window(xcb_connection_t * xc)
{
	xcb_screen_t * s = xstatus_get_screen(xc);
	const int16_t y = s->height_in_pixels
		- XSTATUS_CONST_HEIGHT - XSTATUS_CONST_BORDER;
	const uint32_t vm = XCB_CW_BACK_PIXEL | XCB_CW_OVERRIDE_REDIRECT
		| XCB_CW_EVENT_MASK;
	const uint32_t v[] = {jb_get_pixel(xc, s->default_colormap,
		XSTATUS_PANEL_BACKGROUND), true,
	      XCB_EVENT_MASK_EXPOSURE};
	const xcb_window_t w = xstatus_get_window(xc);
	xcb_create_window(xc, XCB_COPY_FROM_PARENT, w,
		s->root, 0, y, s->width_in_pixels, XSTATUS_CONST_HEIGHT,
		XSTATUS_CONST_BORDER, XCB_WINDOW_CLASS_COPY_FROM_PARENT,
		XCB_COPY_FROM_PARENT, vm, v);
	xcb_map_window(xc, w);
}
static struct XStatusButton *last_btn(void)
{
	struct XStatusButton * i = xstatus_head_button;
	if(i)
		while(i->next)
			i=i->next;
	return i;
}
static uint16_t xstatus_get_button_end(void)
{
	struct XStatusButton * b = last_btn();
	xcb_rectangle_t * restrict g = &b->widget.geometry;
	return g->x + g->width;
}
static uint16_t poll_status(xcb_connection_t * restrict xc)
{
	uint16_t offset = xstatus_get_button_end() + XSTATUS_CONST_PAD;
	offset = xstatus_draw_load(xc, offset);
#ifdef XSTATUS_USE_TEMPERATURE
	offset = draw_temp(xc, offset);
#endif//XSTATUS_USE_TEMPERATURE
	offset = draw_status_file(xc, offset, xstatus_filename);
	return offset;
}
static void update(xcb_connection_t * xc)
{
	const uint16_t width = xstatus_get_screen(xc)->width_in_pixels;
	xcb_clear_area(xc, 0, xstatus_get_window(xc),
		0, 0, width, XSTATUS_CONST_HEIGHT);
#ifdef XSTATUS_USE_BATTERY_BAR
	xstatus_draw_battery(xc, poll_status(xc), xstatus_draw_clock(xc));
#else//!XSTATUS_USE_BATTERY_BAR
	poll_status(xc);
	xstatus_draw_clock(xc);
#endif//XSTATUS_USE_BATTERY_BAR
}
static void system_cb(struct XStatusButton * b)
{
	const char *cmd = b->cb_data;
	if (system(cmd))
		LIBJB_WARN("Cannot execute %s", cmd);
}
static uint16_t btn(xcb_connection_t * xc, const uint16_t offset,
	char * restrict label, char * restrict cmd)
{
	struct XStatusButton * i = last_btn();
	struct XStatusButton * b = xstatus_get_button(xc, &(xcb_rectangle_t){
		.x=offset, .width = xstatus_get_font_size().width
		* strlen(label) + XSTATUS_CONST_WIDE_PAD,
		.height=XSTATUS_CONST_HEIGHT}, label, system_cb, cmd);
	*(i ? &i->next : &xstatus_head_button) = b;
	return offset + b->widget.geometry.width + XSTATUS_CONST_PAD;
}
/* Returns x offset after all buttons added.  */
static uint16_t setup_buttons(xcb_connection_t * xc)
{
	uint16_t off = 0;
	off = btn(xc, off, "Menu", XSTATUS_MENU_COMMAND);
	off = btn(xc, off, "Terminal", XSTATUS_TERMINAL);
	off = btn(xc, off, "Editor", XSTATUS_EDITOR_COMMAND);
	{
		char *browser=getenv("XSTATUS_BROWSER_COMMAND");
		off=btn(xc, off, "Browser",
			browser?browser:XSTATUS_BROWSER_COMMAND);
	}
	off=btn(xc, off, "Mixer", XSTATUS_MIXER_COMMAND);
	off=btn(xc, off, "Lock", XSTATUS_LOCK_COMMAND);
	return off;
}
static struct XStatusButton * find_button(const xcb_window_t w)
{
	for(struct XStatusButton * i = xstatus_head_button; i; i=i->next)
		if(i->widget.window==w)
			return i;
	return NULL;
}
static bool iter_buttons(const xcb_window_t ewin,
	void (*func)(struct XStatusButton * restrict))
{
	struct XStatusButton * b = find_button(ewin);
	if (b) {
		func(b);
		return true;
	}
	return false;
}
// returns if update needed
__attribute__((nonnull))
static void handle_events(xcb_connection_t * xc,
	xcb_generic_event_t * restrict e)
{
	switch (e->response_type) {
	case XCB_EXPOSE:
		if(!iter_buttons(((xcb_expose_event_t *)e)->window,
			xstatus_head_button->draw))
			update(xc);
		break;
	case XCB_BUTTON_PRESS:
		iter_buttons(((xcb_button_press_event_t *)e)->event,
			xstatus_head_button->cb);
		break;
	default:
		LOG("event: %d", e->response_type);
	}
	free(e);
}
__attribute__((noreturn))
static void event_loop(xcb_connection_t * xc, const uint8_t delay)
{
	for (;;) {
		xcb_generic_event_t * e;
		if (jb_next_event_timed(xc, &e, delay * 1000000) && e)
			handle_events(xc, e);
		update(xc);
	}
}
static void setup_font(xcb_connection_t * xc)
{
	if (!xstatus_open_font(xc, XSTATUS_FONT)) // default
		if (!xstatus_open_font(xc, "fixed")) // fallback
			LIBJB_ERROR("Could not load any font");
}
static xcb_connection_t * setup_xdata(void)
{
	xcb_connection_t * xc = jb_get_xcb_connection(NULL, NULL);
	create_window(xc);
	setup_font(xc); // font needed for gc
	xstatus_create_gc(xc, xstatus_get_gc(xc), xstatus_get_window(xc),
		XSTATUS_PANEL_FOREGROUND, XSTATUS_PANEL_BACKGROUND);
	return xc;
}
void xstatus_start(char * restrict filename, const uint8_t delay)
{
	xcb_connection_t * xc = setup_xdata();
	xstatus_create_gc(xc, xstatus_get_button_gc(xc),
		xstatus_get_window(xc), XSTATUS_BUTTON_FG,
		XSTATUS_BUTTON_BG);
	setup_buttons(xc);
	xstatus_filename = filename;
	event_loop(xc, delay);
}

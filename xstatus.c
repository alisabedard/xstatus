// Copyright 2016, Jeffrey E. Bedard
#include "xstatus.h"
#include "battery.h"
#include "button.h"
#include "clock.h"
#include "config.h"
#include "font.h"
#include "libjb/log.h"
#include "libjb/xcb.h"
#include "load.h"
#include "status_file.h"
#include "temperature.h"
#include "util.h"
#include <string.h>
static struct XSButton * xstatus_head_button;
static inline int16_t get_y(xcb_screen_t * restrict s)
{
	return s->height_in_pixels - XSTATUS_CONST_HEIGHT
		- XSTATUS_CONST_BORDER;
}
static void create_window(xcb_connection_t * xc)
{
	xcb_screen_t * s = xstatus_get_screen(xc);
	const xcb_window_t w = xstatus_get_window(xc);
	const uint32_t vm = XCB_CW_BACK_PIXEL
		| XCB_CW_OVERRIDE_REDIRECT | XCB_CW_EVENT_MASK;
	xcb_create_window(xc, XCB_COPY_FROM_PARENT, w,
		s->root, 0, get_y(s), s->width_in_pixels, XSTATUS_CONST_HEIGHT,
		XSTATUS_CONST_BORDER, XCB_WINDOW_CLASS_COPY_FROM_PARENT,
		XCB_COPY_FROM_PARENT, vm, (uint32_t[]){ jb_get_pixel(xc,
		s->default_colormap, XSTATUS_PANEL_BACKGROUND), true,
		XCB_EVENT_MASK_EXPOSURE});
	xcb_map_window(xc, w);
}
static struct XSButton * get_last_button_r(struct XSButton * i)
{
	return i->next ? get_last_button_r(i->next) : i;
}
static struct XSButton *get_last_button(void)
{
	return xstatus_head_button ? get_last_button_r(xstatus_head_button)
		: NULL;
}
static uint16_t xstatus_get_button_end(void)
{
	struct XSButton * restrict b = get_last_button();
	return b->x + b->width;
}
static uint16_t poll_status(xcb_connection_t * restrict xc,
	const char * filename)
{
	uint16_t offset = xstatus_get_button_end() + XSTATUS_CONST_PAD;
	offset = xstatus_draw_load(xc, offset);
#ifdef XSTATUS_USE_TEMPERATURE
	offset = draw_temp(xc, offset);
#endif//XSTATUS_USE_TEMPERATURE
	offset = draw_status_file(xc, offset, filename);
	return offset;
}
static void update(xcb_connection_t * restrict xc,
	const char * restrict filename)
{
	const uint16_t width = xstatus_get_screen(xc)->width_in_pixels;
	xcb_clear_area(xc, 0, xstatus_get_window(xc),
		0, 0, width, XSTATUS_CONST_HEIGHT);
#ifdef XSTATUS_USE_BATTERY_BAR
	xstatus_draw_battery(xc, poll_status(xc, filename),
		xstatus_draw_clock(xc));
#else//!XSTATUS_USE_BATTERY_BAR
	poll_status(xc, filename);
	xstatus_draw_clock(xc);
#endif//XSTATUS_USE_BATTERY_BAR
}
static void system_cb(struct XSButton * b)
{
	const char *cmd = b->cb_data;
	if (system(cmd))
		LIBJB_WARN("Cannot execute %s", cmd);
}
static uint16_t btn(xcb_connection_t * xc, const int16_t offset,
	char * restrict label, char * restrict cmd)
{
	struct XSButton * i = get_last_button();
	struct XSButton * b = xstatus_create_button(xc, offset, label);
	b->cb = system_cb;
	b->cb_data = cmd;
	*(i ? &i->next : &xstatus_head_button) = b;
	return offset + b->width + XSTATUS_CONST_PAD;
}
/* Returns x offset after all buttons added.  */
static uint16_t initialize_buttons(xcb_connection_t * xc)
{
	uint16_t off = 0;
	off = btn(xc, off, "Menu", XSTATUS_MENU_COMMAND);
	off = btn(xc, off, "Terminal", XSTATUS_TERMINAL);
	off = btn(xc, off, "Editor", XSTATUS_EDITOR_COMMAND);
	{ // * browser scope
		char * browser=getenv("XSTATUS_BROWSER_COMMAND");
		off = btn(xc, off, "Browser", browser ? browser
			: XSTATUS_BROWSER_COMMAND);
	}
	off = btn(xc, off, "Mixer", XSTATUS_MIXER_COMMAND);
	off = btn(xc, off, "Lock", XSTATUS_LOCK_COMMAND);
	return off;
}
static struct XSButton * find_button_r(const xcb_window_t w,
	struct XSButton * i)
{
	return i ? i->window == w ? i : find_button_r(w, i->next) : NULL;
}
static bool iterate_buttons(const xcb_window_t ewin,
	void (*func)(struct XSButton * restrict))
{
	struct XSButton * b = find_button_r(ewin, xstatus_head_button);
	if (b) {
		func(b);
		return true;
	}
	return false;
}
static void handle_expose_event(xcb_connection_t * restrict xc,
	xcb_expose_event_t * restrict e, const char * restrict filename)
{
	if(!iterate_buttons(e->window, xstatus_head_button->draw))
		update(xc, filename);
}
static void handle_button_press_event(xcb_button_press_event_t * restrict e)
{
	iterate_buttons(e->event, xstatus_head_button->cb);
}
// returns if update needed
__attribute__((nonnull))
static void handle_events(xcb_connection_t * restrict xc,
	xcb_generic_event_t * restrict e, const char * restrict filename)
{
	switch (e->response_type) {
	case XCB_EXPOSE:
		handle_expose_event(xc, (xcb_expose_event_t *)e, filename);
		break;
	case XCB_BUTTON_PRESS:
		handle_button_press_event((xcb_button_press_event_t *)e);
		break;
	default:
		LOG("event: %d", e->response_type);
	}
	free(e);
}
__attribute__((noreturn))
static void event_loop(xcb_connection_t * restrict xc,
	const uint8_t delay, const char * restrict filename)
{
	for (;;) {
		xcb_generic_event_t * e;
		if (jb_next_event_timed(xc, &e, delay * 1000000) && e)
			handle_events(xc, e, filename);
		else
			update(xc, filename);
	}
}
static void initialize_font(xcb_connection_t * restrict xc)
{
	if (!xstatus_open_font(xc, XSTATUS_FONT)) // default
		if (!xstatus_open_font(xc, "fixed")) // fallback
			LIBJB_ERROR("Could not load any font");
}
static void initialize_gcs(xcb_connection_t * restrict xc)
{
	const xcb_window_t w = xstatus_get_window(xc);
	xstatus_create_gc(xc, xstatus_get_gc(xc), w,
		XSTATUS_PANEL_FOREGROUND, XSTATUS_PANEL_BACKGROUND);
	xstatus_create_gc(xc, xstatus_get_button_gc(xc), w,
		XSTATUS_BUTTON_FG, XSTATUS_BUTTON_BG);
}
void xstatus_start(struct XStatusOptions * restrict opt)
{
	xcb_connection_t * xc = jb_get_xcb_connection(NULL, NULL);
	create_window(xc);
	initialize_font(xc); // font needed for gc
	initialize_gcs(xc);
	initialize_buttons(xc);
	event_loop(xc, opt->delay, opt->filename);
}

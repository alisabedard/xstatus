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
#include <string.h>
// Application state struct
#if defined(XSTATUS_USE_STATUS_FILE) || defined(XSTATUS_USE_BUTTONS)\
	|| defined(XSTATUS_USE_BATTERY_BAR)
static struct {
#ifdef XSTATUS_USE_STATUS_FILE
	char * filename;
#endif//XSTATUS_USE_STATUS_FILE
#ifdef XSTATUS_USE_BUTTONS
	struct XStatusButton * head_button;
#endif//XSTATUS_USE_BUTTONS
	xcb_rectangle_t geometry;
} xstatus;
#endif//XSTATUS_USE_STATUS_FILE||XSTATUS_USE_BUTTONS||XSTATUS_USE_BATTERY_BAR
static void create_window(struct XData * restrict X)
{
	xcb_connection_t * xc = X->xcb;
	X->w = xcb_generate_id(xc);
	xcb_screen_t * s = X->screen;
	X->sz = (xcb_rectangle_t) {
		.y = s->height_in_pixels - XSTATUS_CONST_HEIGHT
			- XSTATUS_CONST_BORDER,
			.width = s->width_in_pixels,
			.height = XSTATUS_CONST_HEIGHT
	};
	const uint32_t vm = XCB_CW_BACK_PIXEL | XCB_CW_OVERRIDE_REDIRECT
		| XCB_CW_EVENT_MASK;
	const xcb_colormap_t cm = s->default_colormap;
	const uint32_t v[] = {jb_get_pixel(xc, cm,
		XSTATUS_PANEL_BACKGROUND), true,
	      XCB_EVENT_MASK_EXPOSURE};
	const xcb_rectangle_t sz = X->sz;
	xcb_create_window(xc, XCB_COPY_FROM_PARENT, X->w,
		s->root, sz.x, sz.y, sz.width,
		sz.height, XSTATUS_CONST_BORDER,
		XCB_WINDOW_CLASS_COPY_FROM_PARENT,
		XCB_COPY_FROM_PARENT, vm, v);
	xcb_map_window(xc, X->w);
}
#ifdef XSTATUS_USE_BUTTONS
static struct XStatusButton *last_btn(void)
{
	struct XStatusButton * i = xstatus.head_button;
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
#else//!XSTATUS_USE_BUTTONS
#define xstatus_get_button_end() 0
#endif//XSTATUS_USE_BUTTONS
#if defined(XSTATUS_USE_LOAD) || defined(USE_BUTTON)\
|| defined(XSTATUS_USE_TEMPERATURE) || defined(XSTATUS_USE_STATUS_FILE)
static uint16_t poll_status(struct XData * restrict X)
{
	uint16_t offset = xstatus_get_button_end() + XSTATUS_CONST_PAD;
#ifdef XSTATUS_USE_LOAD
	offset = xstatus_draw_load(X, offset);
#endif//XSTATUS_USE_LOAD
#ifdef XSTATUS_USE_TEMPERATURE
	offset = draw_temp(X, offset);
#endif//XSTATUS_USE_TEMPERATURE
#ifdef XSTATUS_USE_STATUS_FILE
	offset = draw_status_file(X, offset, xstatus.filename);
#endif//XSTATUS_USE_TEMPERATURE
	return offset;
}
#else
#define poll_status(X)
#endif/*XSTATUS_USE_LOAD||USE_BUTTON
	||XSTATUS_USE_TEMPERATURE||XSTATUS_USE_STATUS_FILE*/
static void update(struct XData * restrict X)
{
	xcb_clear_area(X->xcb, 0, X->w, 0, 0, X->sz.width, X->sz.height);
#ifdef XSTATUS_USE_BATTERY_BAR
#ifdef XSTATUS_USE_LOCK
	xstatus_draw_battery(X, poll_status(X), xstatus_draw_clock(X));
#else//!XSTATUS_USE_LOCK
	xstatus_draw_battery(X, poll_status(X), X->screen->width_in_pixels);
#endif//XSTATUS_USE_LOCK
#else//!XSTATUS_USE_BATTERY_BAR
	poll_status(X);
	xstatus_draw_clock(X);
#endif//XSTATUS_USE_BATTERY_BAR
}
#ifdef XSTATUS_USE_BUTTONS
static void system_cb(struct XStatusButton * b)
{
	const char *cmd = b->cb_data;
	if (system(cmd))
		WARN("Cannot execute %s", cmd);
}
static uint16_t btn(struct XData * restrict X, const uint16_t offset,
	char * restrict label, char * restrict cmd)
{
	struct XStatusButton * i = last_btn();
	struct XStatusButton * b = xstatus_get_button(X, &(xcb_rectangle_t){
		.x=offset, .width = X->font_size.width
		* strlen(label) + XSTATUS_CONST_WIDE_PAD,
		.height=XSTATUS_CONST_HEIGHT}, label, system_cb, cmd);
	*(i ? &i->next : &xstatus.head_button) = b;
	return offset + b->widget.geometry.width + XSTATUS_CONST_PAD;
}
/* Returns x offset after all buttons added.  */
static uint16_t setup_buttons(struct XData * restrict X)
{
	uint16_t off = 0;
	off = btn(X, off, "Menu", XSTATUS_MENU_COMMAND);
	off = btn(X, off, "Terminal", XSTATUS_TERMINAL);
	off = btn(X, off, "Editor", XSTATUS_EDITOR_COMMAND);
	{
		char *browser=getenv("XSTATUS_BROWSER_COMMAND");
		off=btn(X, off, "Browser",
			browser?browser:XSTATUS_BROWSER_COMMAND);
	}
	off=btn(X, off, "Mixer", XSTATUS_MIXER_COMMAND);
	off=btn(X, off, "Lock", XSTATUS_LOCK_COMMAND);
	return off;
}
static struct XStatusButton * find_button(const xcb_window_t w)
{
	for(struct XStatusButton * i = xstatus.head_button; i; i=i->next)
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
static void handle_events(struct XData * restrict X,
	xcb_generic_event_t * restrict e)
{
	switch (e->response_type) {
	case XCB_EXPOSE:
		if(!iter_buttons(((xcb_expose_event_t *)e)->window,
			xstatus.head_button->draw))
			update(X);
		break;
	case XCB_BUTTON_PRESS:
		iter_buttons(((xcb_button_press_event_t *)e)->event,
			xstatus.head_button->cb);
		break;
	default:
		LOG("event: %d", e->response_type);
	}
	free(e);
}
#else//!XSTATUS_USE_BUTTONS
#define handle_events(X, e) {}
#endif//XSTATUS_USE_BUTTONS
__attribute__((noreturn))
static void event_loop(struct XData * restrict X, const uint8_t delay)
{
	for (;;) {
		xcb_generic_event_t * e;
		if (jb_next_event_timed(X->xcb, &e,
			delay * 1000000) && e)
			handle_events(X, e);
		update(X);
	}
}
static void setup_font(struct XData * restrict X)
{
	if (!xstatus_open_font(X->xcb, XSTATUS_FONT)) // default
		if (!xstatus_open_font(X->xcb, "fixed")) // fallback
			ERROR("Could not load any font");
	X->font_size = xstatus_get_font_size();
}
static void setup_xdata(struct XData * X)
{
	X->xcb = jb_get_xcb_connection(NULL, NULL);
	X->font = xstatus_get_font(X->xcb); // get the id
	X->screen = jb_get_xcb_screen(X->xcb);
	create_window(X);
	setup_font(X); // font needed for gc
	X->gc = xstatus_get_gc(X, XSTATUS_PANEL_FOREGROUND,
		XSTATUS_PANEL_BACKGROUND);
}
void xstatus_start(
#ifdef XSTATUS_USE_STATUS_FILE
	char * restrict filename,
#else//!XSTATUS_USE_STATUS_FILE
	char * restrict filename __attribute__((unused)),
#endif//XSTATUS_USE_STATUS_FILE
	const uint8_t delay)
{
	struct XData X;
	setup_xdata(&X);
#ifdef XSTATUS_USE_BUTTONS
	struct XData BX = X;
	BX.gc = xstatus_get_gc(&BX, XSTATUS_BUTTON_FG, XSTATUS_BUTTON_BG);
	setup_buttons(&BX);
#endif//XSTATUS_USE_BUTTONS
#ifdef XSTATUS_USE_STATUS_FILE
	xstatus.filename = filename;
#endif//XSTATUS_USE_STATUS_FILE
	event_loop(&X, delay);
}

// Copyright 2016, Jeffrey E. Bedard

#include "xstatus.h"

#include "battery.h"
#include "button.h"
#include "clock.h"
#include "config.h"
#include "libjb/class.h"
#include "libjb/log.h"
#include "libjb/xcb.h"
#include "load.h"
#include "status_file.h"
#include "temperature.h"

#include <string.h>

// Application state struct
#if defined(USE_STATUS) || defined(USE_BUTTONS)\
	|| defined(USE_BATTERY)
static struct {
#ifdef USE_STATUS
	char * filename;
#endif//USE_STATUS
#ifdef USE_BUTTONS
	struct Button * head_button;
#endif//USE_BUTTONS
	xcb_rectangle_t geometry;
} xstatus;
#endif//USE_STATUS||USE_BUTTONS||USE_BATTERY

static void create_window(struct XData * restrict X)
{
	xcb_connection_t * xc = X->xcb;
	X->w = xcb_generate_id(xc);
	xcb_screen_t * s = X->screen;
	X->sz = (xcb_rectangle_t) {
		.y = s->height_in_pixels - XS_HEIGHT - XS_BORDER,
		.width = s->width_in_pixels,
		.height = XS_HEIGHT
	};
	const uint32_t vm = XCB_CW_BACK_PIXEL | XCB_CW_OVERRIDE_REDIRECT
		| XCB_CW_EVENT_MASK;
	const xcb_colormap_t cm = s->default_colormap;
	const uint32_t v[] = {jb_get_pixel(xc, cm, XSTATUS_PANEL_BACKGROUND), true,
		XCB_EVENT_MASK_EXPOSURE};
	const xcb_rectangle_t sz = X->sz;
	xcb_create_window(xc, XCB_COPY_FROM_PARENT, X->w,
			  s->root, sz.x, sz.y, sz.width,
			  sz.height, XS_BORDER,
			  XCB_WINDOW_CLASS_COPY_FROM_PARENT,
			  XCB_COPY_FROM_PARENT, vm, v);
	xcb_map_window(xc, X->w);
}

#ifdef USE_BUTTONS
static struct Button *last_btn(void)
{
	struct Button * i = xstatus.head_button;
	if(i)
		while(i->next)
			i=i->next;
	return i;
}

static uint16_t get_button_end(void)
{
	struct Button * b = last_btn();
	xcb_rectangle_t * restrict g = &b->widget.geometry;
	return g->x + g->width;
}
#else//!USE_BUTTONS
#define get_button_end() 0
#endif//USE_BUTTONS

#if defined(USE_LOAD) || defined(USE_BUTTON)\
	|| defined(USE_TEMP) || defined(USE_STATUS)
static uint16_t poll_status(struct XData * restrict X)
{
	uint16_t offset = get_button_end() + XS_PAD;
#ifdef USE_LOAD
	offset = draw_load(X, offset);
#endif//USE_LOAD
#ifdef USE_TEMP
	offset = draw_temp(X, offset);
#endif//USE_TEMP
#ifdef USE_STATUS
	offset = draw_status_file(X, offset, xstatus.filename);
#endif//USE_TEMP
	return offset;
}
#else
#define poll_status(X)
#endif//USE_LOAD||USE_BUTTON||USE_TEMP||USE_STATUS

static void update(struct XData * restrict X)
{
#ifdef USE_BATTERY
#ifdef USE_CXSTATUS_LOCK_COMMAND
	draw_battery(X, poll_status(X), draw_clock(X));
#else//!USE_CXSTATUS_LOCK_COMMAND
	draw_battery(X, poll_status(X), X->screen->width_in_pixels);
#endif//USE_CXSTATUS_LOCK_COMMAND
#else//!USE_BATTERY
	poll_status(X);
	draw_clock(X);
#endif//USE_BATTERY
}

#ifdef USE_BUTTONS
static void system_cb(struct Button * b)
{
	const char *cmd = b->cb_data;
	if (system(cmd))
		WARN("Cannot execute %s", cmd);
}

static uint16_t btn(struct XData * restrict X, const uint16_t offset,
	char * restrict label, char * restrict cmd)
{
	struct Button * i = last_btn();
	struct Button * b = get_button(X, &(xcb_rectangle_t){
		.x=offset, .width = X->font_size.width
		* strlen(label) + XS_WPAD,
		.height=XS_HEIGHT}, label, system_cb, cmd);
	*(i ? &i->next : &xstatus.head_button) = b;
	return offset + b->widget.geometry.width + XS_PAD;
}

/* Returns x offset after all buttons added.  */
static uint16_t setup_buttons(struct XData * restrict X)
{
	uint16_t off = 0;
	off = btn(X, off, "Menu", XS_MENU);
	off = btn(X, off, "Terminal", XSTATUS_TERMINAL);
	off = btn(X, off, "Editor", EDITOR);
	{
		char *browser=getenv("XSTATUS_BROWSER_COMMAND");
		off=btn(X, off, "Browser",
			browser?browser:XSTATUS_BROWSER_COMMAND);
	}
	off=btn(X, off, "Mixer", MIXER);
	off=btn(X, off, "Lock", XSTATUS_LOCK_COMMAND);
	return off;
}

static struct Button * find_button(const xcb_window_t w)
{
	for(struct Button * i = xstatus.head_button; i; i=i->next)
		  if(i->widget.window==w)
			    return i;
	return NULL;
}

static bool iter_buttons(const xcb_window_t ewin,
	void (*func)(struct Button * restrict))
{
	struct Button * b = find_button(ewin);
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
#else//!USE_BUTTONS
#define handle_events(X, e) {}
#endif//USE_BUTTONS

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

static bool open_font(struct XData * restrict X, const char * fn)
{
	xcb_connection_t * xc = X->xcb;
	xcb_void_cookie_t c = xcb_open_font_checked(xc, X->font,
		strlen(fn), fn);
	xcb_query_font_cookie_t fc = xcb_query_font(xc, X->font);
	if (jb_xcb_cookie_has_error(xc, c)) {
		WARN("Failed to load font: %s", fn);
		return false;
	}
	xcb_query_font_reply_t * r = xcb_query_font_reply(xc, fc, NULL);
	{
		xcb_charinfo_t * ci = &r->max_bounds;
		X->font_size.width = ci->character_width;
		X->font_size.height = ci->ascent + ci->descent;
	}
	free(r);
	return true;
}

static void setup_font(struct XData * restrict X)
{
	X->font = xcb_generate_id(X->xcb);
	if (open_font(X, FONT)) // default
		return;
	if (open_font(X, "fixed")) // fallback
		return;
	ERROR("Could not load any font");
}

static void setup_xdata(struct XData * X)
{
	X->xcb = jb_get_xcb_connection(NULL, NULL);
	X->screen = jb_get_xcb_screen(X->xcb);
	create_window(X);
	setup_font(X); // font needed for gc
	X->gc = xcbgc(X, XSTATUS_PANEL_FOREGROUND, XSTATUS_PANEL_BACKGROUND);
}

void run_xstatus(
#ifdef USE_STATUS
	char * restrict filename,
#else//!USE_STATUS
	char * restrict filename __attribute__((unused)),
#endif//USE_STATUS
	const uint8_t delay)
{
	struct XData X;
	setup_xdata(&X);
#ifdef USE_BUTTONS
	struct XData BX = X;
	BX.gc = xcbgc(&BX, XSTATUS_BUTTON_FG, XSTATUS_BUTTON_BG);
	setup_buttons(&BX);
#endif//USE_BUTTONS
//	setup_battery(&xstatus.bat, &X);
#ifdef USE_STATUS
	xstatus.filename = filename;
#endif//USE_STATUS
	event_loop(&X, delay);
}


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
	Button * head_button;
#endif//USE_BUTTONS
#ifdef USE_BATTERY
	Battery bat;
#endif//USE_BATTERY
	xcb_rectangle_t geometry;
} xstatus;
#endif//USE_STATUS||USE_BUTTONS||USE_BATTERY

static void create_window(XData * restrict X)
{
	xcb_connection_t * xc = X->xcb;
	X->w = xcb_generate_id(xc);
	X->sz = (xcb_rectangle_t) { .y
		= X->screen->height_in_pixels - HEIGHT
			- BORDER, .width
			= X->screen->width_in_pixels,
			.height = HEIGHT};
	const uint32_t vm = XCB_CW_BACK_PIXEL | XCB_CW_OVERRIDE_REDIRECT
		| XCB_CW_EVENT_MASK;
	const xcb_colormap_t cm = X->screen->default_colormap;
	const uint32_t v[] = {jb_get_pixel(xc, cm, PANEL_BG), true,
		XCB_EVENT_MASK_EXPOSURE};
	const xcb_rectangle_t sz = X->sz;
	xcb_create_window(xc, XCB_COPY_FROM_PARENT, X->w,
			  X->screen->root, sz.x, sz.y, sz.width,
			  sz.height, BORDER,
			  XCB_WINDOW_CLASS_COPY_FROM_PARENT,
			  XCB_COPY_FROM_PARENT, vm, v);
	xcb_map_window(xc, X->w);
}

#ifdef USE_BUTTONS
static Button *last_btn(void)
{
	Button * i = xstatus.head_button;
	if(i)
		while(i->next)
			i=i->next;
	return i;
}

static uint16_t get_button_end(void)
{
	Button * b = last_btn();
	xcb_rectangle_t * restrict g = &b->widget.geometry;
	return g->x + g->width;
}
#else//!USE_BUTTONS
#define get_button_end() 0
#endif//USE_BUTTONS

#if defined(USE_LOAD) || defined(USE_BUTTON)\
	|| defined(USE_TEMP) || defined(USE_STATUS)
static uint16_t poll_status(XData * restrict X)
{
	uint16_t offset = get_button_end() + PAD;
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

#if 0
static inline void clear(XData * restrict X)
{
	xcb_clear_area(X->xcb, false, X->w, 0, 0,
		X->sz.width, X->sz.height);
	xcb_flush(X->xcb);
}
#endif

__attribute__ ((hot))
static void update(XData * restrict X)
{
//	clear(X);
#ifdef USE_BATTERY
	xstatus.bat.x.begin=poll_status(X);
	xstatus.bat.x.end=draw_clock(X);
	xcb_clear_area(X->xcb, false, X->w, xstatus.bat.x.begin, 0,
		xstatus.bat.x.end - xstatus.bat.x.begin, X->sz.height);
	xcb_flush(X->xcb);
	// Depends on x values set in clock and status
	$(&xstatus.bat, draw);
#else//!USE_BATTERY
	poll_status(X);
	draw_clock(X);
#endif//USE_BATTERY
	xcb_flush(X->xcb);
}

#ifdef USE_BUTTONS
static void system_cb(Button * b)
{
	const char *cmd = b->cb_data;
	if (system(cmd))
		WARN("Cannot execute %s", cmd);
}

static uint16_t btn(XData * restrict X, const uint16_t offset,
	char * restrict label, char * restrict cmd)
{
	Button * i = last_btn();
	Button * b = get_button(X, &(xcb_rectangle_t){
		.x=offset, .width = X->font_width
		* strlen(label)+(PAD<<1),
		.height=HEIGHT}, label, system_cb, cmd);
	*(i ? &i->next : &xstatus.head_button) = b;
	return offset + b->widget.geometry.width + PAD;
}

/* Returns x offset after all buttons added.  */
static uint16_t setup_buttons(XData * restrict X)
{
	uint16_t off = 0;
	off = btn(X, off, "Menu", MENU);
	off = btn(X, off, "Terminal", TERM);
	off = btn(X, off, "Editor", EDITOR);
	{
		char *browser=getenv("BROWSER");
		off=btn(X, off, "Browser",
			browser?browser:BROWSER);
	}
	off=btn(X, off, "Mixer", MIXER);
	off=btn(X, off, "Lock", LOCK);
	return off;
}

static Button * find_button(const xcb_window_t w)
{
	for(Button * i = xstatus.head_button; i; i=i->next)
		  if(i->widget.window==w)
			    return i;
	return NULL;
}

static void iter_buttons(const xcb_window_t ewin,
	void (*func)(Button * restrict))
{
	Button * b = find_button(ewin);
	if (b) func(b);
}
#endif//USE_BUTTONS

__attribute__((noreturn))
static void event_loop(XData * restrict X, const uint8_t delay)
{
	xcb_generic_event_t * e;
	xcb_expose_event_t * expose;
	xcb_button_press_event_t * button;
eventl:
	if (next_event_timed(X, &e, delay)) {
#ifdef USE_BUTTONS
		switch (e->response_type) {
		case XCB_EXPOSE:
			expose = (xcb_expose_event_t *)e;
			iter_buttons(expose->window,
				     xstatus.head_button->draw);
			break;
		case XCB_BUTTON_PRESS:
			button = (xcb_button_press_event_t *)e;
			iter_buttons(button->event,
				     xstatus.head_button->cb);
			break;
		default:
			LOG("event: %d", e->response_type);
		}
		free(e);
#endif//USE_BUTTONS
	}
	update(X);
	goto eventl;
}

static bool open_font(XData * restrict X, const char * fn)
{
	xcb_void_cookie_t c;
	xcb_query_font_cookie_t fc;
	xcb_query_font_reply_t * r;
	xcb_charinfo_t * ci;
	xcb_generic_error_t * e;
	c = xcb_open_font_checked(X->xcb, X->font,
		strlen(fn), fn);
	fc = xcb_query_font(X->xcb, X->font);
	if ((e = xcb_request_check(X->xcb, c))) {
		WARN("Failed to load font: %s", fn);
		free(e);
		return false;
	}
	r = xcb_query_font_reply(X->xcb, fc, NULL);
	ci = &r->max_bounds;
	X->font_width = ci->character_width;
	X->font_height = ci->ascent + ci->descent;
	free(r);
	return true;
}

static void setup_font(XData * restrict X)
{
	X->font = xcb_generate_id(X->xcb);
	if (open_font(X, FONT)) // default
		return;
	if (open_font(X, "fixed")) // fallback
		return;
	ERROR("Could not load any font");
}

static void setup_xdata(XData * X)
{
	X->xcb = jb_get_xcb_connection(NULL, NULL);
	X->screen = jb_get_xcb_screen(X->xcb);
	create_window(X);
	setup_font(X); // font needed for gc
	X->gc = xcbgc(X, PANEL_FG, PANEL_BG);
}

void run_xstatus(
#ifdef USE_STATUS
	char * restrict filename,
#else//!USE_STATUS
	char * restrict filename __attribute__((unused)),
#endif//USE_STATUS
	const uint8_t delay)
{
	XData X;
	setup_xdata(&X);
#ifdef USE_BUTTONS
	XData BX = X;
	BX.gc = xcbgc(&BX, BUTTON_FG, BUTTON_BG);
	setup_buttons(&BX);
#endif//USE_BUTTONS
	setup_battery(&xstatus.bat, &X);
#ifdef USE_STATUS
	xstatus.filename = filename;
#endif//USE_STATUS
	event_loop(&X, delay);
}


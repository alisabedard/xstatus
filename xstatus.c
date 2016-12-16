// Copyright 2016, Jeffrey E. Bedard
#include "xstatus.h"
#include "battery.h"
#include "clock.h"
#include "config.h"
#include "font.h"
#include "libjb/log.h"
#include "libjb/xcb.h"
#include "load.h"
#include "status_file.h"
#include "temperature.h"
#include "toolbar.h"
#include "util.h"
#include "window.h"
#include "xdata.h"
static uint16_t poll_status(xcb_connection_t * restrict xc,
	const char * filename, const uint16_t widget_start)
{
	uint16_t offset = widget_start + XSTATUS_CONST_PAD;
	offset = xstatus_draw_load(xc, offset);
	offset = draw_temp(xc, offset);
	offset = draw_status_file(xc, offset, filename);
	return offset;
}
static void update(xcb_connection_t * restrict xc,
	const char * restrict filename, const uint16_t widget_start)
{
	xstatus_draw_battery(xc, poll_status(xc, filename, widget_start),
		xstatus_draw_clock(xc));
}
// returns if update needed
__attribute__((nonnull))
static void handle_events(xcb_connection_t * restrict xc,
	xcb_generic_event_t * restrict e, const char * restrict filename,
	const uint16_t widget_start)
{
	switch (e->response_type) {
	case XCB_ENTER_NOTIFY:
#if LOG_LEVEL > 8
		LOG("enter");
#endif//LOG_LEVEL
		xstatus_toolbar_handle_button_enter(
			((xcb_enter_notify_event_t*)e)->event);
		break;
	case XCB_LEAVE_NOTIFY:
#if LOG_LEVEL > 8
		LOG("leave");
#endif//LOG_LEVEL
		xstatus_toolbar_handle_button_leave(
			((xcb_leave_notify_event_t*)e)->event);
		break;
	case XCB_EXPOSE:
		if (!xstatus_toolbar_handle_expose(((xcb_expose_event_t*)e)
			->window))
			update(xc, filename, widget_start);
		break;
	case XCB_BUTTON_PRESS:
		xstatus_toolbar_handle_button_press(
			((xcb_button_press_event_t *)e)->event);
		break;
	default:
		LOG("event: %d", e->response_type);
	}
	free(e);
}
__attribute__((noreturn))
static void event_loop(xcb_connection_t * restrict xc,
	const uint8_t delay, const char * restrict filename,
	const uint16_t widget_start)
{
	xcb_generic_event_t * e;
event_loop:
	if (jb_next_event_timed(xc, &e, delay * 1000000) && e)
		handle_events(xc, e, filename, widget_start);
	else
		update(xc, filename, widget_start);
	goto event_loop;
}
static void initialize_font(xcb_connection_t * restrict xc)
{
	if (!xstatus_open_font(xc, XSTATUS_FONT)) // default
		if (!xstatus_open_font(xc, "fixed")) // fallback
			LIBJB_ERROR("Could not load any font");
}
static void setup_invert_gc(xcb_connection_t * restrict xc,
	const xcb_window_t w)
{
	xcb_gcontext_t gc = xstatus_get_invert_gc(xc);
	xcb_create_gc(xc, gc, w, XCB_GC_FUNCTION,
		(uint32_t[]){XCB_GX_INVERT});
}
static void initialize_gcs(xcb_connection_t * restrict xc)
{
	const xcb_window_t w = xstatus_get_window(xc);
	xstatus_create_gc(xc, xstatus_get_gc(xc), w,
		XSTATUS_PANEL_FOREGROUND, XSTATUS_PANEL_BACKGROUND);
	xstatus_create_gc(xc, xstatus_get_button_gc(xc), w,
		XSTATUS_BUTTON_FG, XSTATUS_BUTTON_BG);
	setup_invert_gc(xc, w);
}
static uint16_t initialize(xcb_connection_t * restrict xc)
{
	xstatus_create_window(xc);
	initialize_font(xc); // font needed for gc
	initialize_gcs(xc);
	return xstatus_initialize_toolbar(xc);
}
void xstatus_start(struct XStatusOptions * restrict opt)
{
	xcb_connection_t * xc = jb_get_xcb_connection(NULL, NULL);
	event_loop(xc, opt->delay, opt->filename, initialize(xc));
}

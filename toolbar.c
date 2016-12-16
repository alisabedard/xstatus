// Copyright 2016, Jeffrey E. Bedard
#include "toolbar.h"
#include "button.h"
#include "config.h"
#include "libjb/log.h"
#include <stdlib.h>
static struct XSButton * xstatus_head_button;
static struct XSButton * get_last_button_r(struct XSButton * i)
{
	return i->next ? get_last_button_r(i->next) : i;
}
static struct XSButton * get_last_button(void)
{
	return xstatus_head_button ? get_last_button_r(xstatus_head_button)
		: NULL;
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
uint16_t xstatus_initialize_toolbar(xcb_connection_t * xc)
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
bool xstatus_toolbar_handle_expose(const xcb_window_t event_window)
{
	return iterate_buttons(event_window, xstatus_head_button->draw);
}
bool xstatus_toolbar_handle_button_press(const xcb_window_t event_window)
{
	return iterate_buttons(event_window, xstatus_head_button->cb);
}
bool xstatus_toolbar_handle_button_enter(const xcb_window_t event_window)
{
	return iterate_buttons(event_window, xstatus_head_button->enter);
}
bool xstatus_toolbar_handle_button_leave(const xcb_window_t event_window)
{
	return iterate_buttons(event_window, xstatus_head_button->leave);
}

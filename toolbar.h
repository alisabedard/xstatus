// Copyright 2016, Jeffrey E. Bedard
#ifndef XSTATUS_TOOLBAR_H
#define XSTATUS_TOOLBAR_H
#include <stdbool.h>
#include <xcb/xcb.h>
/* Returns x offset after all buttons added.  */
uint16_t xstatus_initialize_toolbar(xcb_connection_t * xc);
bool xstatus_toolbar_handle_button_enter(const xcb_window_t event_window);
bool xstatus_toolbar_handle_button_leave(const xcb_window_t event_window);
// returns true if event was processed:
bool xstatus_toolbar_handle_button_press(const xcb_window_t event_window);
// returns true if event was processed:
bool xstatus_toolbar_handle_expose(const xcb_window_t event_window);
#endif//!XSTATUS_TOOLBAR_H

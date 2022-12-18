// Copyright 2017, Alisa Bedard
#ifndef XSTATUS_TOOLBAR_H
#define XSTATUS_TOOLBAR_H
#include "XSXData.h"
#include <xcb/xcb.h>
/* Returns x offset after all buttons added.  */
uint16_t xstatus_initialize_toolbar(struct XSXData * X);
void xstatus_toolbar_handle_button_enter(const xcb_window_t event_window);
// returns true if event was processed:
void xstatus_toolbar_handle_button_press(const xcb_window_t event_window);
// returns true if event was processed:
void xstatus_toolbar_handle_expose(const xcb_window_t event_window);
#endif//!XSTATUS_TOOLBAR_H

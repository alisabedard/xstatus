// Copyright 2016, Jeffrey E. Bedard
#ifndef XSTATUS_STATUS_FILE_H
#define XSTATUS_STATUS_FILE_H
#include <stdint.h>
#include <xcb/xcb.h>
// Returns offset for next widget
uint16_t draw_status_file(xcb_connection_t * xc,
	const uint16_t x_offset,
	const char * restrict filename);
#endif//!XSTATUS_STATUS_FILE_H

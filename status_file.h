// Copyright 2017, Jeffrey E. Bedard
#ifndef XSTATUS_STATUS_FILE_H
#define XSTATUS_STATUS_FILE_H
#include <stdint.h>
#include <xcb/xcb.h>
// Returns offset for next widget
int draw_status_file(xcb_connection_t * xc, short x, const char * filename);
#endif//!XSTATUS_STATUS_FILE_H

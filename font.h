// Copyright 2017, Jeffrey E. Bedard
#ifndef XSTATUS_FONT_H
#define XSTATUS_FONT_H
#include "XSXData.h"
#include "libjb/JBDim.h"
#include <xcb/xcb.h>
#include <stdbool.h>
struct JBDim xstatus_get_font_size(void);
void xstatus_open_font(struct XSXData * restrict X, const char * restrict fn);
#endif//!XSTATUS_FONT_H

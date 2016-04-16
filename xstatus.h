#ifndef XSTATUS_H
#define XSTATUS_H

#include <stdint.h>
#include <X11/Xlib.h>

uint16_t xstatus_get_end(void);

const XFontStruct * xstatus_get_font(void);

#endif//!XSTATUS_H

#ifndef XSTATUS_XSTATUS_H
#define XSTATUS_XSTATUS_H
#include "XStatusOptions.h"
#include "XSXData.h"
#include <xcb/xcb.h>
void xstatus_create_gc(struct XSXData * restrict x,
  const xcb_gcontext_t gc, const char * restrict fg, const char * restrict bg);
void xstatus_start(struct XStatusOptions * opt);
#endif//!XSTATUS_XSTATUS_H

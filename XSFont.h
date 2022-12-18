// Copyright 2017-2020, Alisa Bedard
// This file contains the font data structure.
#ifndef XSTATUS_XSFONT_H
#define XSTATUS_XSFONT_H
#include <xcb/xcb.h>
struct XSFont {
  xcb_font_t Font;
  uint8_t Width;
  uint8_t Height;
};
#endif // !XSTATUS_XSFONT_H

// Copyright 2017, Jeffrey E. Bedard
#ifndef XSTATUS_BUTTON_H
#define XSTATUS_BUTTON_H
#include "XSButton.h"
#include <stdint.h>
struct XSButton * createButton(struct XSXData * restrict XData,
  char * Label, int16_t const XPosition);
#endif//XSTATUS_BUTTON_H

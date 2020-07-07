// Copyright 2017, Jeffrey E. Bedard
#ifndef XSTATUS_XSTEXTWIDGET_H
#define XSTATUS_XSTEXTWIDGET_H
#include "XSXData.h"
struct XSTextWidget {
  struct XSXData * x;
  char * buffer;
  short buffer_size;
  short offset;
};
#endif//!XSTATUS_XSTEXTWIDGET_H

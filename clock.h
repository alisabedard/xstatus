// Copyright 2016, Jeffrey E. Bedard

#ifndef XS_CXSTATUS_LOCK_COMMAND_H
#define XS_CXSTATUS_LOCK_COMMAND_H

#ifdef USE_CXSTATUS_LOCK_COMMAND

#include "xdata.h"

uint16_t draw_clock(struct XData * restrict X);

#else//!USE_CXSTATUS_LOCK_COMMAND
#define draw_clock(X)
#endif//USE_CXSTATUS_LOCK_COMMAND

#endif//!XS_CXSTATUS_LOCK_COMMAND_H

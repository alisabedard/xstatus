#ifndef XS_CLOCK_H
#define XS_CLOCK_H

#ifdef USE_CLOCK

#include <stdint.h>
#include "xdata.h"

uint16_t draw_clock(XData * restrict X);

#else//!USE_CLOCK
#define draw_clock(X)
#endif//USE_CLOCK

#endif//!XS_CLOCK_H

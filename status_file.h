#ifndef XSTATUS_STATUS_FILE_H
#define XSTATUS_STATUS_FILE_H

#ifdef USE_STATUS
#include "xdata.h"
#include <stdint.h>

// Returns offset for next widget
uint16_t draw_status_file(struct XData * restrict X,
	const uint16_t x_offset,
	const char * restrict filename);

#else//!USE_STATUS
#define draw_status_file(X, o, f) o
#endif//USE_STATUS

#endif//!XSTATUS_STATUS_FILE_H

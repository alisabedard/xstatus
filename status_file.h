#ifndef XSTATUS_STATUS_FILE_H
#define XSTATUS_STATUS_FILE_H

#ifdef XSTATUS_USE_STATUS_FILE
#include "xdata.h"
#include <stdint.h>

// Returns offset for next widget
uint16_t draw_status_file(struct XData * restrict X,
	const uint16_t x_offset,
	const char * restrict filename);

#else//!XSTATUS_USE_STATUS_FILE
#define draw_status_file(X, o, f) o
#endif//XSTATUS_USE_STATUS_FILE

#endif//!XSTATUS_STATUS_FILE_H

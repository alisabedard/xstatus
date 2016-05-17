#ifndef XS_STATUS_FILE
#define XS_STATUS_FILE

#include "xdata.h"
#include <stdint.h>

// Returns offset for next widget
uint16_t draw_status_file(XData * restrict X, const uint16_t x_offset,
	const char * restrict filename);

#endif//!XS_STATUS_FILE

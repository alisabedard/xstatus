#ifndef XSTATUS_XSTATUS_H
#define XSTATUS_XSTATUS_H

#include <stdint.h>

void run_xstatus(
#ifdef XSTATUS_USE_STATUS_FILE
	char * restrict filename,
#else//!XSTATUS_USE_STATUS_FILE
	char * restrict filename __attribute__((unused)),
#endif//XSTATUS_USE_STATUS_FILE
	const uint8_t delay);

#endif//!XSTATUS_XSTATUS_H

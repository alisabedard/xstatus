#ifndef XSTATUS_H
#define XSTATUS_H

#include <stdint.h>

void run_xstatus(
#ifdef USE_STATUS
	char * restrict filename,
#else//!USE_STATUS
	char * restrict filename __attribute__((unused)),
#endif//USE_STATUS
	const uint8_t delay);

#endif//!XSTATUS_H

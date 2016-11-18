#ifndef XSTATUS_XSTATUS_H
#define XSTATUS_XSTATUS_H
#include <stdint.h>
struct XStatusOptions {
	char * filename;
	uint8_t delay;
};
void xstatus_start(struct XStatusOptions * opt);
#endif//!XSTATUS_XSTATUS_H

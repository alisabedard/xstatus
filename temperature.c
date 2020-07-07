// Copyright 2017, Jeffrey E. Bedard
#include "temperature.h"
#include <stdio.h>
#include "XSTextWidget.h"
#include "config.h"
#include "font.h"
#include "text_widget.h"
#include "util.h"
static uint8_t get_temp(void)
{
	// may only fail once:
	static bool get_temp_failed;
	if (get_temp_failed)
		return 0; // 0 indicates unsupported
	// type must hold at least 100000, signed
	int32_t temp = xstatus_system_value(XSTATUS_SYSFILE_TEMPERATURE);
	if (temp == -1) { // could not open system file
		get_temp_failed = true;
		return 0;
	}
	return temp / 1000;
}
static uint8_t format(char * restrict buf, const uint8_t sz)
{
	const uint8_t temp = get_temp();
	return temp ? snprintf(buf, sz, "%dC", temp) : 0;
}
// Returns x offset for next item
uint16_t draw_temp(struct XSXData * restrict xdata, const uint16_t offset)
{
	uint8_t sz = 4;
	const int16_t x = offset + XSTATUS_CONST_PAD;
	char buf[sz];
	sz = format(buf, sz);
	return xstatus_draw_text_widget(&(struct XSTextWidget){
          xdata, buf, sz, x});
}

// Copyright 2017, Jeffrey E. Bedard
#include "font.h"
#include <stdlib.h>
#include "libjb/xcb.h"
#include "xdata.h"
static struct JBDim font_size;
struct JBDim xstatus_get_font_size(void)
{
	return font_size;
}
__attribute__((pure))
static struct JBDim charinfo_to_size(xcb_charinfo_t * restrict ci)
{
	return (struct JBDim) { .width = ci->character_width,
		.height = ci->ascent + ci->descent };
}
static void finish_query(xcb_connection_t * restrict xc,
	const xcb_query_font_cookie_t fc)
{
	xcb_query_font_reply_t * r = xcb_query_font_reply(xc, fc, NULL);
	font_size = charinfo_to_size(&r->max_bounds);
	free(r);
}
// returns true if successful
bool xstatus_open_font(xcb_connection_t * restrict xc,
	const char * restrict fn)
{
	xcb_font_t f = xstatus_get_font(xc);
	if(!jb_open_font(xc, f, fn))
		return false;
	finish_query(xc, xcb_query_font(xc, f));
	return true;
}


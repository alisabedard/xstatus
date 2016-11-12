// Copyright 2016, Jeffrey E. Bedard
#include "font.h"
#include "libjb/log.h"
#include "libjb/xcb.h"
#include <stdlib.h>
#include <string.h>
static struct JBDim font_size;
xcb_window_t xstatus_get_window(xcb_connection_t * restrict xc)
{
	static xcb_window_t w;
	return w ? w : (w = xcb_generate_id(xc));
}
xcb_font_t xstatus_get_font(xcb_connection_t * restrict xc)
{
	static xcb_font_t f;
	return f ? f : (f = xcb_generate_id(xc));
}
struct JBDim xstatus_get_font_size(void)
{
	return font_size;
}
static void charinfo_to_size(xcb_charinfo_t * restrict ci)
{
	font_size.width = ci->character_width;
	font_size.height = ci->ascent + ci->descent;
}
static void finish_query(xcb_connection_t * restrict xc,
	const xcb_query_font_cookie_t fc)
{
	xcb_query_font_reply_t * r = xcb_query_font_reply(xc, fc, NULL);
	charinfo_to_size(&r->max_bounds);
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


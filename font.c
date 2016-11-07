// Copyright 2016, Jeffrey E. Bedard
#include "font.h"
#include "libjb/log.h"
#include "libjb/xcb.h"
#include <stdlib.h>
#include <string.h>
static struct JBDim font_size;
xcb_font_t xstatus_get_font(xcb_connection_t * restrict xc)
{
	static xcb_font_t f;
	return f ? f : (f = xcb_generate_id(xc));
}
struct JBDim xstatus_get_font_size(void)
{
	return font_size;
}
// returns true if successful
bool xstatus_open_font(xcb_connection_t * restrict xc,
	const char * restrict fn)
{
	xcb_font_t f = xstatus_get_font(xc);
	xcb_void_cookie_t c = xcb_open_font_checked(xc, f, strlen(fn), fn);
	xcb_query_font_cookie_t fc = xcb_query_font(xc, f);
	if (jb_xcb_cookie_has_error(xc, c)) {
		WARN("Failed to load font: %s", fn);
		return false;
	}
	xcb_query_font_reply_t * r = xcb_query_font_reply(xc, fc, NULL);
	{
		xcb_charinfo_t * ci = &r->max_bounds;
		font_size.width = ci->character_width;
		font_size.height = ci->ascent + ci->descent;
	}
	free(r);
	return true;
}


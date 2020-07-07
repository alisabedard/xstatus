// Copyright 2017, Jeffrey E. Bedard
#include "font.h"
#include <stdlib.h>
#include "libjb/log.h"
#include "libjb/xcb.h"
#include "XSXData.h"
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
void xstatus_open_font(struct XSXData * restrict x,
	const char * restrict fn)
{
	if(!jb_open_font(x->xc, x->font, fn)
          || !jb_open_font(x->xc, x->font, "fixed"))
			LIBJB_ERROR("Could not load any font");
        // set up font_size
	finish_query(x->xc, xcb_query_font(x->xc, x->font));
}


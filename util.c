// Copyright 2016, Jeffrey E. Bedard

#include "util.h"

#include "libjb/log.h"
#include "libjb/util.h"
#include "libjb/xcb.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

xcb_gc_t xcbgc(struct XData * restrict X, char * fg, char * bg)
{
	xcb_connection_t * xc = X->xcb;
	const xcb_gc_t gc = xcb_generate_id(xc);
	const xcb_colormap_t cm = X->screen->default_colormap;
	xcb_void_cookie_t c = xcb_create_gc_checked(xc, gc, X->w,
		XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT,
		(uint32_t[]){jb_get_pixel(xc, cm, fg),
		jb_get_pixel(xc, cm, bg), X->font});
	xcb_generic_error_t * e = xcb_request_check(xc, c);
	if (jb_check(!e, "Could not create GC"))
		free(e);
	return gc;
}

#if defined(USE_BATTERY) || defined(USE_TEMP)
uint32_t sysval(const char *filename)
{
	FILE *f = fopen(filename, "r");
	if (jb_check(f, "Cannot open file"))
		return 0;
	size_t sz = 6;
	char buf[sz];
	fread(buf, 1, sz, f);
	jb_check(fclose(f) == 0, "Cannot close file");
	return atoi(buf);
}
#endif//USE_BATTERY||USE_TEMP


// Copyright 2016, Jeffrey E. Bedard

#include "util.h"

#include "log.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

Pixel pixel(XData * restrict X, const char * restrict color)
{
	xcb_alloc_named_color_cookie_t c
		= xcb_alloc_named_color(X->xcb,
			X->screen->default_colormap,
			strlen(color), color);
	xcb_alloc_named_color_reply_t * r
		= xcb_alloc_named_color_reply(X->xcb, c, NULL);
	if (!r) {
		WARN("Could not allocate color %s", color);
		return X->screen->black_pixel;
	}
	Pixel p = r->pixel;
	free(r);
	return p;
}

xcb_gc_t xcbgc(XData * restrict X, char * fg, char * bg)
{
	xcb_gc_t gc = xcb_generate_id(X->xcb);
	xcb_create_gc(X->xcb, gc, X->w, XCB_GC_FOREGROUND
		| XCB_GC_BACKGROUND | XCB_GC_FONT,
		(uint32_t[]){pixel(X, fg), pixel(X, bg),
		X->font});
	return gc;
}

#if defined(USE_BATTERY) || defined(USE_TEMP)
uint32_t sysval(const char *filename)
{
	FILE *f = fopen(filename, "r");
	if(!f) {
		WARN("Cannot open %s", filename);
		return 0;
	}
	char buf[6];
	size_t sz = fread(&buf, 1, 6, f);
	fclose(f);
	if(!sz)
		  return 0;
	return atoi(buf);
}
#endif//USE_BATTERY||USE_TEMP

bool next_event_timed(XData * restrict X,
	xcb_generic_event_t ** restrict e,
	const uint8_t delay)
{
	*e = xcb_poll_for_event(X->xcb);
	if (*e)
		  return true;
	int fd = xcb_get_file_descriptor(X->xcb);
	fd_set r;
	FD_ZERO(&r);
	FD_SET(fd, &r);
	if (!select(fd + 1, &r, NULL, NULL,
		&(struct timeval){.tv_sec = delay}))
		return false; // timeout
	// event occurred before timeout:
	*e = xcb_poll_for_event(X->xcb);
	return true;
}


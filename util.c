// Copyright 2016, Jeffrey E. Bedard

#include "util.h"

#include "log.h"

#include <stdlib.h>
#include <string.h>

Display * get_display()
{
	Display *d = XOpenDisplay(NULL);
	if (!d) ERROR("Cannot open DISPLAY\n");
	return d;
}

Pixel pixel(XData * restrict X, const char * restrict color)
{
	xcb_alloc_named_color_cookie_t c
		= xcb_alloc_named_color(X->xcb,
			X->screen->default_colormap,
			strlen(color), color);
	xcb_generic_error_t * e;
	xcb_alloc_named_color_reply_t * r
		= xcb_alloc_named_color_reply(X->xcb,
			c, &e);
	if (e) {
		WARN("Could not alloc color %s", color);
		free(e);
	}
	Pixel p = r->pixel;
	free(r);
	return p;
}


xcb_gc_t xcbgc(XData * restrict X, const char * restrict
	color)
{
	xcb_gc_t gc = xcb_generate_id(X->xcb);
	xcb_create_gc(X->xcb, gc, X->w, XCB_GC_FOREGROUND
		| XCB_GC_FONT, (uint32_t[]){pixel(X, color),
		X->font});
	return gc;
}

#if defined(USE_BATTERY) || defined(USE_TEMP)
uint32_t sysval(const char *filename)
{
	FILE *f = fopen(filename, "r");
	if(!f) {
		WARN("Cannot open %s\n", filename);
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

bool XNextEventTimed(Display * dsp, XEvent * event_return,
	const uint8_t delay)
{
	if (!XPending(dsp)) {
		int fd = ConnectionNumber(dsp);
		fd_set readset;
		FD_ZERO(&readset);
		FD_SET(fd, &readset);
		if (!select(fd + 1, &readset, NULL, NULL,
			&(struct timeval){.tv_sec=delay}))
			return false;
		else
			goto xnext;
	} else {
 xnext:
		XNextEvent(dsp, event_return);
		return true;
	}
}


// Copyright 2016, Jeffrey E. Bedard
#include "util.h"

#include "log.h"

#include <stdlib.h>
#include <string.h>
#if 0
#include <assert.h>

#include <stdlib.h>
#include <sys/select.h>
#include "config.h"
#include "log.h"
#include "util.h"
#include "xdata.h"
#endif

uint16_t font_y(XFontStruct * restrict f)
{
	return f->ascent+f->descent;
}

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
	Pixel p = r->pixel;
	free(r);
	return p;
}

GC colorgc(XData * restrict X, const char * restrict color)
{
	XGCValues gv = {.foreground = pixel(X, color),
		.font = X->font->fid};
	return XCreateGC(X->d, X->w,
		GCForeground | GCFont, &gv);
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


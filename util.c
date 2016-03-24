#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <X11/Xlib.h>
#include "config.h"
#include "xstatus.h"
#include "util.h"

uint16_t font_y(void)
{
	const XFontStruct * restrict f = xstatus_font;
	return f->ascent+f->descent;
}

uint16_t string_width(const size_t sz)
{
	const uint8_t w = xstatus_font->max_bounds.width;
	return (sz+1)*w;
}

Display * get_display()
{
	Display *d = XOpenDisplay(NULL);
	if (!d) {
		fputs("Cannot open DISPLAY\n", stderr);
		exit(1);
	}
	return d;
}

unsigned long pixel(Display * restrict d, const char * restrict color)
{
	XColor c, nc;
	XAllocNamedColor(d, DefaultColormap(d, 0), color, &c, &nc);
	return c.pixel;
}

GC colorgc(Display * restrict d, const Window w, const char * restrict color)
{
	assert(xstatus_font);
	assert(xstatus_font->fid);
	XGCValues gv = {.foreground = pixel(d, color),
		.font = xstatus_font->fid
	};
	return XCreateGC(d, w, GCForeground | GCFont, &gv);
}

int sysval(const char *filename)
{
	FILE *f = fopen(filename, "r");
	if(!f) {
		WARN("Cannot open %s\n", filename);
		return 0;
	}
	char buf[4];
	fread(&buf, 1, 4, f);
	fclose(f);
	return atoi(buf);
}

bool XNextEventTimed(Display * dsp, XEvent * event_return)
{
	struct timeval tv = { DELAY, 0 };
	if (!XPending(dsp)) {
		int fd = ConnectionNumber(dsp);
		fd_set readset;
		FD_ZERO(&readset);
		FD_SET(fd, &readset);
		if (!select(fd + 1, &readset, NULL, NULL, &tv))
			return false;
		else
			goto xnext;
	} else {
 xnext:
		XNextEvent(dsp, event_return);
		return true;
	}
}


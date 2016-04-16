#include <assert.h>
#include <stdlib.h>
#include <sys/select.h>
#include "config.h"
#include "log.h"
#include "xstatus.h"
#include "util.h"

uint16_t font_y(void)
{
	const XFontStruct * restrict f = xstatus_get_font();
	return f->ascent+f->descent;
}

uint16_t string_width(const size_t sz)
{
	const XFontStruct * restrict f = xstatus_get_font();
	return (sz+1)*f->max_bounds.width;
}

Display * get_display()
{
	Display *d = XOpenDisplay(NULL);
	if (!d) ERROR("Cannot open DISPLAY\n");
	return d;
}

Pixel pixel(Display * restrict d, const char * restrict color)
{
	XColor c, nc;
	XAllocNamedColor(d, DefaultColormap(d, 0), color, &c, &nc);
	return c.pixel;
}

GC colorgc(Display * restrict d, const Window w, const char * restrict color)
{
	const XFontStruct * restrict f = xstatus_get_font();
	XGCValues gv = {.foreground = pixel(d, color), .font = f->fid};
	return XCreateGC(d, w, GCForeground | GCFont, &gv);
}

uint32_t sysval(const char *filename)
{
	FILE *f = fopen(filename, "r");
	if(!f) {
		WARN("Cannot open %s\n", filename);
		return 0;
	}
	char buf[4];
	size_t sz = fread(&buf, 1, 4, f);
	fclose(f);
	if(!sz)
		  return 0;
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


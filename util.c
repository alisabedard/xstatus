#include <assert.h>
#include <stdlib.h>
#include <sys/select.h>
#include "config.h"
#include "log.h"
#include "util.h"
#include "xdata.h"

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

Pixel pixel(Display * restrict d, const char * restrict color)
{
	XColor c;
	XAllocNamedColor(d, DefaultColormap(d, 0), color, &c, &(XColor){});
	return c.pixel;
}

GC colorgc(XData * restrict X, const char * restrict color)
{
	XGCValues gv = {.foreground = pixel(X->d, color), .font = X->font->fid};
	return XCreateGC(X->d, X->w, GCForeground | GCFont, &gv);
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


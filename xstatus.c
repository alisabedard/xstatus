#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <X11/Xlib.h>
#include "battery.h"
#include "button.h"
#include "clock.h"
#include "class.h"
#include "config.h"
#include "log.h"
#include "util.h"
#include "xstatus.h"

// Application state struct
static struct {
	uint16_t end;
	char * filename;
	XFontStruct * font;
	Button * head_button;
	Battery bat;
} xstatus;

uint16_t xstatus_get_end(void)
{
	return xstatus.end;
}

const XFontStruct * xstatus_get_font(void)
{
	return xstatus.font;
}

static Window create_window(Display * d)
{
	XSetWindowAttributes a = {.override_redirect = True,
		.background_pixel = pixel(d, PANEL_BG)
	};
#define CFP CopyFromParent
	const Window w = XCreateWindow(d, DefaultRootWindow(d), 0,
		DisplayHeight(d, 0) - HEIGHT - BORDER, DisplayWidth(d, 0),
		HEIGHT, BORDER, CFP, CFP, CFP, CWOverrideRedirect 
		| CWBackPixel, &a);
	XSelectInput(d, w, ExposureMask);
	XMapWindow(d, w);

	return w;
}

static Button *last_btn(void)
{
	Button * i = xstatus.head_button;
	if(!i)
		  return NULL;
	while(i->next)
		  i=i->next;
	return i;
}

#ifdef DRAW_LOAD
// Returns x offset for next item
static uint16_t draw_load(Display * restrict d, const Window w, const GC gc,
	const uint16_t x)
{
	double l[1];
	getloadavg(l, 1);
	static const uint16_t sz=6;
	char buf[sz];
	snprintf(buf, sz, "%.2f", l[0]);
	XDrawString(d, w, gc, x+PAD+1, font_y(), buf, strlen(buf));
	return string_width(sz-2) + x ;
}
#endif//DRAW_LOAD

static uint16_t get_button_end(void)
{
	Button * b = last_btn();
	XRectangle * restrict g = &b->widget.geometry;
	return g->x + g->width;
}

static uint16_t poll_status_file(Display * restrict d, const Window w, 
	const GC gc, const uint16_t x)
{
	assert(xstatus.filename);
	FILE * f = fopen(xstatus.filename, "a+");
	if (!f) ERROR("Cannot open %s\n", xstatus.filename);
	static const uint8_t sz=80;
	char buf[sz];
	// File must end in a newline or extra space.  
	// -1 to remove end terminator.  
	const size_t rsz = fread(&buf, 1, sz, f) - 1;
	fclose(f);
	XDrawString(d, w, gc, x+PAD, font_y(), buf, rsz); 
	LOG("buf is %lu\n", strlen(buf));

	return string_width(rsz) + x;
}

static void poll_status(Display * restrict d, const Window w, const GC gc)
{
	uint16_t x = get_button_end() + PAD;
#ifdef USE_LOAD
	x = draw_load(d, w, gc, x);
#endif
	xstatus.end = poll_status_file(d, w, gc, x);
}

__attribute__ ((hot))
static void update(Display * d, const Window w, const GC gc)
{
	XClearWindow(d, w);
	draw_clock(d, w, gc);
	poll_status(d, w, gc);
	// Depends on x values set in clock and status
	$(&xstatus.bat, draw);
}

static void system_cb(Button * b)
{
	const char *cmd = b->cb_data;
	if (system(cmd))
		WARN("Cannot execute %s\n", cmd);
}

static uint16_t btn(Display * restrict d, const Window w, const GC gc,
	const uint16_t x, char * restrict label, char * restrict cmd)
{
	Button * i = last_btn();
	Button * b = new_Button(d, w, gc, x, label, system_cb, cmd);
	if(!i)
		  xstatus.head_button=b;
	else
		  i->next=b;
	return x+b->widget.geometry.width;
}

/* Returns x offset after all buttons added.  */
static uint16_t setup_buttons(Display * restrict d, const Window w, const GC gc)
{
	uint16_t x=0;
#define BTN(l, c) x=btn(d, w, gc, x, l, c)
	BTN("Menu", MENU);
	BTN("Terminal", TERM);
	BTN("Editor", EDITOR);
	{
		char *browser=getenv("BROWSER");
		BTN("Browser", browser?browser:BROWSER);
	}
	BTN("Mixer", MIXER);
	BTN("Lock", LOCK);
	Button * i = last_btn();
	XRectangle * g = &i->widget.geometry;
	return g->x+g->width;
}

static Button * find_button(const Window w)
{
	for(Button * i = xstatus.head_button; i; i=i->next)
		  if(i->widget.window==w)
			    return i;
	return NULL;
}

static void iter_buttons(const Window ewin, void (*func)(Button * restrict))
{
	Button * restrict b = find_button(ewin);
	if(b)
		  func(b);
}

__attribute__((noreturn))
static void event_loop(Display * restrict d, const Window w,
	const GC gc)
{
	XEvent e;
 eventl:
	if (XNextEventTimed(d, &e)) {
		switch (e.type) {
		case Expose:
			iter_buttons(e.xexpose.window, xstatus.head_button->draw);
			break;
		case ButtonPress:
			iter_buttons(e.xbutton.window, xstatus.head_button->cb);
			break;
		default:
			LOG("event: %d\n", e.type);
		}
	}
	update(d, w, gc);
	goto eventl;
}

static void setup_font(Display * restrict d)
{
	xstatus.font=XLoadQueryFont(d, FONT);
	if(!xstatus.font)
		xstatus.font=XLoadQueryFont(d, "fixed");
	if(!xstatus.font)
		ERROR("Failed to load font");
}

int main(int argc, char ** argv)
{
	Display * d = get_display();
	setup_font(d);
	const Window w = create_window(d);
	GC gc=colorgc(d, w, PANEL_FG);
	GC bgc=colorgc(d, w, BUTTON_FG);
	setup_buttons(d, w, bgc);
	xstatus.filename = argc > 1 ? argv[2] : DEFAULTF;
	setup_battery(&xstatus.bat, d, w);
	event_loop(d, w, gc);
}

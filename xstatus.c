#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <X11/Xlib.h>
#include "battery.h"
#include "button.h"
#include "clock.h"
#include "config.h"
#include "log.h"
#include "util.h"
#include "xstatus.h"

/* FIXME: Work to eliminate these globals.  */

XFontStruct *xstatus_font;
uint16_t xstatus_status_w;

// Static environment structure, used as a namespace.  
static struct {
	Battery bat;
#define NBTN 5
	Button * btns [NBTN];
} xstatus;

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

__attribute__ ((hot))
static void update(Display * d, const Window w, const GC gc, 
	const char *filename)
{
	XClearWindow(d, w);
	draw_clock(d, w, gc);
	FILE *f = fopen(filename, "a+");
	if (!f) ERROR("Cannot open %s\n", filename);
	char buf[80];
	// File must end in a newline or extra space.  
	const size_t rsz = fread(&buf, 1, sizeof buf, f);
	fclose(f);
	xstatus_status_w = string_width(rsz) + xstatus_row_x + BUTTON_SPACE;
	XDrawString(d, w, gc, xstatus_row_x + BUTTON_SPACE, font_y(), buf,
		rsz-1); // -1 to remove end terminator.  
	LOG("buf is %lu\n", strlen(buf));
	xstatus.bat.draw(&xstatus.bat);
}


static void
setup_buttons(Display * restrict d, const Window w, const GC gc)
{
	uint8_t n=0;
#define BTN(l, c) xstatus.btns[n++]=cmd_Button(d, w, gc, l, c)
	BTN("Menu", MENU);
	BTN("Terminal", TERM);
	BTN("Editor", EDITOR);
	{
		char *browser=getenv("BROWSER");
		BTN("Browser", browser?browser:BROWSER);
	}
	BTN("Mixer", MIXER);
}

static Button * find_button(const Window w)
{
	for(uint8_t i=0; i<NBTN; i++)
		  if(xstatus.btns[i]->widget.window == w)
			    return xstatus.btns[i];
	return NULL; // not_found
}

static void do_cb(Button * restrict b)
{
	b->cb(b);
}

static void iter_buttons(const Window ewin, void (*func)(Button * restrict))
{
	Button * restrict b = find_button(ewin);
	if(b)
		  func(b);
}

__attribute__((noreturn))
static void event_loop(Display * restrict d, const Window w,
	const GC gc, const char *filename)
{
	XEvent e;
 eventl:
	if (XNextEventTimed(d, &e)) {
		switch (e.type) {
		case Expose:
			iter_buttons(e.xexpose.window, &draw_Button);
			break;
		case ButtonPress:
			iter_buttons(e.xbutton.window, &do_cb);
			break;
		default:
			LOG("event: %d\n", e.type);
		}
	}
	update(d, w, gc, filename);
	goto eventl;
}

int main(int argc, char ** argv)
{
	Display * d = get_display();
	xstatus_font=XLoadQueryFont(d, FONT);
	if(!xstatus_font)
		xstatus_font=XLoadQueryFont(d, "fixed");
	if(!xstatus_font) ERROR("Failed to load font");
	const Window w = create_window(d);
	GC gc=colorgc(d, w, PANEL_FG);
	GC bgc=colorgc(d, w, BUTTON_FG);
	setup_buttons(d, w, bgc);
	const char *filename = argc > 1 ? argv[2] : DEFAULTF;
	setup_battery(&xstatus.bat, d, w);
	event_loop(d, w, gc, filename);
}

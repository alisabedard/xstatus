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
//static uint16_t xstatus_row_x;
uint16_t xstatus_status_w;

static struct {
	Button * head_button;
	Battery bat;
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

static Button *last_btn(void)
{
	Button * i = xstatus.head_button;
	if(!i)
		  return NULL;
	while(i->next)
		  i=i->next;
	return i;
}

__attribute__ ((hot))
static void update(Display * d, const Window w, const GC gc, 
	const char *filename)
{
	XClearWindow(d, w);
	draw_clock(d, w, gc);
	FILE * f = fopen(filename, "a+");
	if (!f) ERROR("Cannot open %s\n", filename);
	char buf[80];
	// File must end in a newline or extra space.  
	const size_t rsz = fread(&buf, 1, sizeof buf, f);
	fclose(f);
	Button * b = last_btn();
	XRectangle * g = &b->widget.geometry;
	const uint16_t bx = g->x + g->width + PAD;
	xstatus_status_w = string_width(rsz) + bx + PAD;
	XDrawString(d, w, gc, bx + PAD, font_y(), buf,
		rsz-1); // -1 to remove end terminator.  
	LOG("buf is %lu\n", strlen(buf));
	xstatus.bat.draw(&xstatus.bat);
}

static uint16_t btn(Display * restrict d, const Window w, const GC gc,
	const uint16_t x, char * restrict label, char * restrict cmd)
{
	Button * i = last_btn();
	Button * b = cmd_Button(d, w, gc, x, label, cmd);
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

static void setup_font(Display * restrict d)
{
	xstatus_font=XLoadQueryFont(d, FONT);
	if(!xstatus_font)
		xstatus_font=XLoadQueryFont(d, "fixed");
	if(!xstatus_font)
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
	const char *filename = argc > 1 ? argv[2] : DEFAULTF;
	setup_battery(&xstatus.bat, d, w);
	event_loop(d, w, gc, filename);
}

#include "xstatus.h"

#include "battery.h"
#include "button.h"
#include "class.h"
#include "clock.h"
#include "config.h"
#include "load.h"
#include "log.h"
#include "status_file.h"
#include "temperature.h"
#include "util.h"
#include "xstatus.h"
#include "xdata.h"

#include <string.h>

// Application state struct
static struct {
	char * filename;
	Button * head_button;
	Battery bat;
} xstatus;

static Window create_window(Display * d)
{
#define CFP CopyFromParent
	const Window w = XCreateWindow(d, DefaultRootWindow(d), 0,
		DisplayHeight(d, 0) - HEIGHT - BORDER, DisplayWidth(d, 0),
		HEIGHT, BORDER, CFP, CFP, CFP, CWOverrideRedirect
		| CWBackPixel, &(XSetWindowAttributes){.override_redirect=True,
		.background_pixel = pixel(d, PANEL_BG)});
	XSelectInput(d, w, ExposureMask);
	XMapWindow(d, w);

	return w;
}

static Button *last_btn(void)
{
	Button * i = xstatus.head_button;
	if(i)
		while(i->next)
		  	i=i->next;
	return i;
}

static uint16_t get_button_end(void)
{
	Button * b = last_btn();
	XRectangle * restrict g = &b->widget.geometry;
	return g->x + g->width;
}

static uint16_t poll_status(XData * restrict X)
{
	uint16_t offset = get_button_end() + PAD;
	offset = draw_load(X, offset);
	offset = draw_temp(X, offset);
	offset = draw_status_file(X, offset, xstatus.filename);
	return offset;
}

__attribute__ ((hot))
static void update(XData * restrict X)
{
	XClearWindow(X->d, X->w);
	xstatus.bat.x.begin=poll_status(X);
	xstatus.bat.x.end=draw_clock(X);
	// Depends on x values set in clock and status
	$(&xstatus.bat, draw);
}

static void system_cb(Button * b)
{
	const char *cmd = b->cb_data;
	if (system(cmd))
		WARN("Cannot execute %s\n", cmd);
}

static uint16_t btn(XData * restrict X, const uint16_t offset,
	char * restrict label, char * restrict cmd)
{
	Button * i = last_btn();
	Button * b = new_Button(X, &(XRectangle){.x=offset,
		.width=XTextWidth(X->font, label, strlen(label))+(PAD<<1),
		.height=HEIGHT}, label, system_cb, cmd);
	if(!i)
		  xstatus.head_button=b;
	else
		  i->next=b;
	return offset + b->widget.geometry.width + PAD;
}

/* Returns x offset after all buttons added.  */
static uint16_t setup_buttons(XData * restrict X)
{
	uint16_t off = 0;
	off=btn(X, off, "Menu", MENU);
	off=btn(X, off, "Terminal", TERM);
	off=btn(X, off, "Editor", EDITOR);
	{
		char *browser=getenv("BROWSER");
		off=btn(X, off, "Browser", browser?browser:BROWSER);
	}
	off=btn(X, off, "Mixer", MIXER);
	off=btn(X, off, "Lock", LOCK);
	return off;
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
static void event_loop(XData * restrict X, const uint8_t delay)
{
	XEvent e;
 eventl:
	if (XNextEventTimed(X->d, &e, delay)) {
		switch (e.type) {
		case Expose:
			iter_buttons(e.xexpose.window,
				xstatus.head_button->draw);
			break;
		case ButtonPress:
			iter_buttons(e.xbutton.window,
				xstatus.head_button->cb);
			break;
		default:
			LOG("event: %d\n", e.type);
		}
	}
	update(X);
	goto eventl;
}

static void setup_font(XData * restrict X)
{
	X->font=XLoadQueryFont(X->d, FONT);
	if(!X->font)
		X->font=XLoadQueryFont(X->d, "fixed");
	if(!X->font)
		ERROR("Failed to load font");
}

static void setup_xdata(XData * X)
{
	X->d = get_display();
	X->w = create_window(X->d);
	setup_font(X); // font needed for gc
	X->gc = colorgc(X, PANEL_FG);
}

void run_xstatus(char * restrict filename, const uint8_t delay)
{
	XData X;
	setup_xdata(&X);
	setup_buttons(&(XData){.d=X.d,.w=X.w,.font=X.font,
		.gc=colorgc(&X,BUTTON_FG)});
	setup_battery(&xstatus.bat, &X);
	xstatus.filename = filename;
	event_loop(&X, delay);
}


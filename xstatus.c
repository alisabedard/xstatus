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
#include <X11/Xlib-xcb.h>

// Application state struct
#if defined(USE_STATUS) || defined(USE_BUTTONS)\
	|| defined(USE_BATTERY)
static struct {
#ifdef USE_STATUS
	char * filename;
#endif//USE_STATUS
#ifdef USE_BUTTONS
	Button * head_button;
#endif//USE_BUTTONS
#ifdef USE_BATTERY
	Battery bat;
#endif//USE_BATTERY
	xcb_rectangle_t geometry;
} xstatus;
#endif//USE_STATUS||USE_BUTTONS||USE_BATTERY

static void create_window(XData * restrict X)
{
	X->w = xcb_generate_id(X->xcb);
	X->sz = (xcb_rectangle_t) { .y
		= X->screen->height_in_pixels - HEIGHT
			- BORDER, .width
			= X->screen->width_in_pixels,
			.height = HEIGHT};
	xcb_create_window(X->xcb, X->screen->root_depth,
		X->w, X->screen->root, X->sz.x, X->sz.y,
		X->sz.width, X->sz.height, BORDER,
		XCB_WINDOW_CLASS_COPY_FROM_PARENT,
		X->screen->root_visual, XCB_CW_BACK_PIXEL
		| XCB_CW_OVERRIDE_REDIRECT
		| XCB_CW_EVENT_MASK, (uint32_t[]){
		pixel(X, PANEL_BG), true,
		XCB_EVENT_MASK_EXPOSURE});
	xcb_map_window(X->xcb, X->w);
}

#ifdef USE_BUTTONS
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
#else//!USE_BUTTONS
#define get_button_end() 0
#endif//USE_BUTTONS

#if defined(USE_LOAD) || defined(USE_BUTTON)\
	|| defined(USE_TEMP) || defined(USE_STATUS)
static uint16_t poll_status(XData * restrict X)
{
	uint16_t offset = get_button_end() + PAD;
#ifdef USE_LOAD
	offset = draw_load(X, offset);
#endif//USE_LOAD
#ifdef USE_TEMP
	offset = draw_temp(X, offset);
#endif//USE_TEMP
#ifdef USE_STATUS
	offset = draw_status_file(X, offset, xstatus.filename);
#endif//USE_TEMP
	return offset;
}
#else
#define poll_status(X)
#endif//USE_LOAD||USE_BUTTON||USE_TEMP||USE_STATUS

__attribute__ ((hot))
static void update(XData * restrict X)
{
	XClearWindow(X->d, X->w);
#ifdef USE_BATTERY
	xstatus.bat.x.begin=poll_status(X);
	xstatus.bat.x.end=draw_clock(X);
	// Depends on x values set in clock and status
	$(&xstatus.bat, draw);
#else//!USE_BATTERY
	poll_status(X);
	draw_clock(X);
#endif//USE_BATTERY
}

#ifdef USE_BUTTONS
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
		.width=XTextWidth(X->font, label,
			strlen(label))+(PAD<<1),
		.height=HEIGHT}, label, system_cb, cmd);
	*(i ? &i->next : &xstatus.head_button) = b;
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
		off=btn(X, off, "Browser",
			browser?browser:BROWSER);
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

static void iter_buttons(const Window ewin,
	void (*func)(Button * restrict))
{
	Button * restrict b = find_button(ewin);
	if(b)
		  func(b);
}
#endif//USE_BUTTONS

__attribute__((noreturn))
static void event_loop(XData * restrict X, const uint8_t delay)
{
	XEvent e;
 eventl:
	if (XNextEventTimed(X->d, &e, delay)) {
#ifdef USE_BUTTONS
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
#endif//USE_BUTTONS
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
	X->xcb = XGetXCBConnection(X->d);
	X->screen = xcb_setup_roots_iterator(
		xcb_get_setup(X->xcb)).data;
	create_window(X);
	setup_font(X); // font needed for gc
	X->gc = colorgc(X, PANEL_FG);
}

void run_xstatus(
#ifdef USE_STATUS
	char * restrict filename,
#else//!USE_STATUS
	char * restrict filename __attribute__((unused)),
#endif//USE_STATUS
	const uint8_t delay)
{
	XData X;
	setup_xdata(&X);
#ifdef USE_BUTTONS
	XData BX = X;
	BX.gc = colorgc(&BX, BUTTON_FG);
	setup_buttons(&BX);
#endif//USE_BUTTONS
	setup_battery(&xstatus.bat, &X);
#ifdef USE_STATUS
	xstatus.filename = filename;
#endif//USE_STATUS
	event_loop(&X, delay);
}


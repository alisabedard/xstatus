// Copyright 2017, Alisa Bedard
#ifndef LIBJB_XCB_H
#define LIBJB_XCB_H
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus
#include <stdbool.h>
#include <xcb/xcb.h>
typedef uint32_t pixel_t;
/* Make sure there is no error on the xcb connection.  If there is,
   print a descriptive error message then exit.  */
void jb_check_x(xcb_connection_t * x);
/* Create a gc with foreground and background as specified.
   If gc is passed as 0, a new gc value is generated and returned.  */
#ifdef __cplusplus
xcb_gcontext_t jb_create_gc(xcb_connection_t * xc, xcb_gcontext_t gc,
	const xcb_window_t win, const char * fg, const char * bg);
#else//!__cplusplus
xcb_gcontext_t jb_create_gc(xcb_connection_t * xc, xcb_gcontext_t gc,
	const xcb_window_t win, const char * restrict fg,
	const char * restrict bg);
#endif//__cplusplus
xcb_atom_t jb_get_atom(xcb_connection_t * x, const char * name);
xcb_colormap_t jb_get_colormap(xcb_connection_t * xc,
	const xcb_window_t win);
/* Get the cursor specified by name.  Note, specifying name as restrict
 * qualified here breaks C++ builds.  */
xcb_cursor_t jb_get_cursor(xcb_connection_t * xc,
	const char * name);
xcb_colormap_t jb_get_default_colormap(xcb_connection_t * xc);
pixel_t jb_get_pixel(xcb_connection_t * x, const xcb_colormap_t cmap,
	const char * color);
pixel_t jb_get_rgb_pixel(xcb_connection_t * x, const xcb_colormap_t cm,
	const uint16_t r, const uint16_t g, const uint16_t b);
xcb_connection_t * jb_get_xcb_connection(const char * display, int * screen);
xcb_screen_t * jb_get_xcb_screen(xcb_connection_t * x);
bool jb_next_event_timed(xcb_connection_t * x,
	xcb_generic_event_t ** e, const uint32_t delay);
// Open font specified by name.  initialized fid must be supplied
bool jb_open_font(xcb_connection_t * xc, xcb_font_t fid,
	const char * name);
pixel_t jb_set_bg(xcb_connection_t * x, const xcb_gcontext_t gc, const pixel_t p);
pixel_t jb_set_fg(xcb_connection_t * x, const xcb_gcontext_t gc, const pixel_t p);
void jb_set_icon_name(xcb_connection_t * xc, const xcb_window_t win,
	char * name);
pixel_t jb_set_named_bg(xcb_connection_t * xc, const xcb_gcontext_t gc,
	const char * color);
pixel_t jb_set_named_fg(xcb_connection_t * xc, const xcb_gcontext_t gc,
	const char * color);
void jb_set_window_name(xcb_connection_t * xc, const xcb_window_t win,
	char * name);
bool jb_wait_until_event(xcb_connection_t * x,
  const uint32_t delay);
bool jb_xcb_cookie_has_error(xcb_connection_t * x,
	const xcb_void_cookie_t c);
#ifdef __cplusplus
}
#endif//__cplusplus
#endif//!LIBJB_XCB_H

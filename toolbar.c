// Copyright 2017, Jeffrey E. Bedard
#include "toolbar.h"
#include "button.h"
#include "config.h"
#include "libjb/log.h"
#include "libjb/util.h"
static struct XSButton * xstatus_head_button;
static struct XSButton * get_last_button_r(struct XSButton * i)
{
  return i->next ? get_last_button_r(i->next) : i;
}
static struct XSButton * get_last_button(void)
{
  return xstatus_head_button ? get_last_button_r(xstatus_head_button)
  : NULL;
}
static void exit_cb(struct XSButton * b __attribute__((unused)))
{
  exit(0);
}
static void system_cb(struct XSButton * b)
{
  jb_system(b->cb_data);
}
static uint16_t addButton(struct XSXData * X, const int16_t Offset,
  char * restrict Label, void (*Callback)(struct XSButton *),
  char * restrict Data)
{
  struct XSButton * i = get_last_button();
  struct XSButton * b = createButton(X, Label, Offset);
  b->cb = Callback;
  b->cb_data = Data;
  *(i ? &i->next : &xstatus_head_button) = b;
  return Offset + b->widget.Geometry[2] + XSTATUS_CONST_PAD;
}
/* Returns x offset after all buttons added.  */
uint16_t xstatus_initialize_toolbar(struct XSXData * X)
{
  uint16_t Offset = 0;
#ifdef XSTATUS_MINIMAL_TOOLBAR
#ifdef XSTATUS_TERMINAL_COMMAND
  Offset=addButton(X,Offset,"vt",system_cb, XSTATUS_TERMINAL_COMMAND);
#endif//XSTATUS_TERMINAL_COMMAND
#ifdef XSTATUS_LOCK_COMMAND
  Offset=addButton(X,Offset,"lk",system_cb, XSTATUS_LOCK_COMMAND);
#endif//XSTATUS_LOCK_COMMAND
#else//!system_cb, XSTATUS_MINIMAL_TOOLBAR
#ifdef XSTATUS_FM_COMMAND
  Offset = addButton(X, Offset, "Files", system_cb, XSTATUS_FM_COMMAND);
#endif//XSTATUS_FM_COMMAND
#ifdef XSTATUS_TERMINAL_COMMAND
  Offset = addButton(X, Offset, "Terminal", system_cb,
    XSTATUS_TERMINAL_COMMAND);
#endif//XSTATUS_TERMINAL_COMMMAND
#ifdef XSTATUS_EDITOR_COMMAND
  Offset = addButton(X, Offset, "Editor", system_cb, XSTATUS_EDITOR_COMMAND);
#endif//XSTATUS_EDITOR_COMMAND
#ifdef XSTATUS_BROWSER_COMMAND
  Offset = addButton(X, Offset, "Browser", system_cb, XSTATUS_BROWSER_COMMAND);
#endif//XSTATUS_BROWSER_COMMAND
#ifdef XSTATUS_MAIL_COMMAND
  Offset=addButton(X,Offset,"Mail", system_cb, XSTATUS_MAIL_COMMAND);
#endif//XSTATUS_MAIL_COMMAND
#ifdef XSTATUS_MIXER_COMMAND
  Offset = addButton(X, Offset, "Mixer", system_cb, XSTATUS_MIXER_COMMAND);
#endif//XSTATUS_MIXER_COMMAND
#ifdef XSTATUS_LOCK_COMMAND
  Offset = addButton(X, Offset, "Lock", system_cb, XSTATUS_LOCK_COMMAND);
#endif//XSTATUS_LOCK_COMMAND
#ifdef XSTATUS_LOGOUT_COMMAND
  Offset = addButton(X, Offset, "Logout", exit_cb, XSTATUS_LOGOUT_COMMAND);
#endif//XSTATUS_LOGOUT_COMMAND

#endif//XSTATUS_MINIMAL_TOOLBAR
  return Offset;
}
static struct XSButton * find_button_r(const xcb_window_t w,
  struct XSButton * i)
{
  return i ? i->widget.Window == w ? i
  : find_button_r(w, i->next) : NULL;
}
void xstatus_toolbar_handle_expose(const xcb_window_t EventWindow)
{
  struct XSButton * B;
  B = find_button_r(EventWindow, xstatus_head_button);
  if (B)
    B->draw(B);
}
void xstatus_toolbar_handle_button_press(const xcb_window_t EventWindow)
{
  struct XSButton * B;
  B = find_button_r(EventWindow, xstatus_head_button);
  if (B)
    B->cb(B);
}
void xstatus_toolbar_handle_button_enter(const xcb_window_t EventWindow)
{
  struct XSButton * B;
  B = find_button_r(EventWindow, xstatus_head_button);
  if (B)
    B->enter(B);
}

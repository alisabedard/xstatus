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
static void system_cb(struct XSButton * b)
{
  bool is_exit;
  uint8_t sz,i;
  char const *cmd = b->cb_data;
  char const exit_cmd[]="__EXIT__";
  uint8_t const exit_cmd_sz = sizeof(exit_cmd);
  is_exit=false;
  sz=0;
  while((cmd[sz++]) && (sz<=exit_cmd_sz))
    ;
  if(sz==exit_cmd_sz){
    for(i=0,is_exit=true; i<sz; ++i){
      if(cmd[i]!=exit_cmd[i]){
        is_exit=false;
        break;
      }
    }
  }
#ifdef DEBUG_SYSTEM_CB
  fprintf(stderr, "cmd: %s, sz: %d, is_exit: %d\n", cmd, sz, is_exit);
#endif//DEBUG_SYSTEM_CB
  if(is_exit){
    exit(0);
  } else {
    jb_system(cmd);
  }
}
static uint16_t btn(struct XSXData * X, const int16_t offset,
  char * restrict label, char * restrict cmd)
{
  struct XSButton * i = get_last_button();
  struct XSButton * b = createButton(X, label, offset);
  b->cb = system_cb;
  b->cb_data = cmd;
  *(i ? &i->next : &xstatus_head_button) = b;
  return offset + b->widget.Geometry[2] + XSTATUS_CONST_PAD;
}
/* Returns x offset after all buttons added.  */
uint16_t xstatus_initialize_toolbar(struct XSXData * X)
{
  uint16_t off = 0;
#ifdef XSTATUS_MINIMAL_TOOLBAR
#ifdef XSTATUS_TERMINAL_COMMAND
  off=btn(X,off,"vt",XSTATUS_TERMINAL_COMMAND);
#endif//XSTATUS_TERMINAL_COMMAND
#ifdef XSTATUS_LOCK_COMMAND
  off=btn(X,off,"lk",XSTATUS_LOCK_COMMAND);
#endif//XSTATUS_LOCK_COMMAND
#else//!XSTATUS_MINIMAL_TOOLBAR
#ifdef XSTATUS_FM_COMMAND
  off = btn(X, off, "Files", XSTATUS_FM_COMMAND);
#endif//XSTATUS_FM_COMMAND
#ifdef XSTATUS_TERMINAL_COMMAND
  off = btn(X, off, "Terminal", XSTATUS_TERMINAL_COMMAND);
#endif//XSTATUS_TERMINAL_COMMMAND
#ifdef XSTATUS_EDITOR_COMMAND
  off = btn(X, off, "Editor", XSTATUS_EDITOR_COMMAND);
#endif//XSTATUS_EDITOR_COMMAND
#ifdef XSTATUS_BROWSER_COMMAND
  { // * browser scope
    char * browser=getenv("XSTATUS_BROWSER_COMMAND");
    off = btn(X, off, "Browser", browser ? browser
      : XSTATUS_BROWSER_COMMAND);
  }
#endif//XSTATUS_BROWSER_COMMAND
#ifdef XSTATUS_MAIL_COMMAND
  off=btn(X,off,"Mail",XSTATUS_MAIL_COMMAND);
#endif//XSTATUS_MAIL_COMMAND
#ifdef XSTATUS_MIXER_COMMAND
  off = btn(X, off, "Mixer", XSTATUS_MIXER_COMMAND);
#endif//XSTATUS_MIXER_COMMAND
#ifdef XSTATUS_LOCK_COMMAND
  off = btn(X, off, "Lock", XSTATUS_LOCK_COMMAND);
#endif//XSTATUS_LOCK_COMMAND
#ifdef XSTATUS_LOGOUT_COMMAND
  off = btn(X, off, "Logout", XSTATUS_LOGOUT_COMMAND);
#endif//XSTATUS_LOGOUT_COMMAND

#endif//XSTATUS_MINIMAL_TOOLBAR
  return off;
}
static struct XSButton * find_button_r(const xcb_window_t w,
  struct XSButton * i)
{
  return i ? i->widget.Window == w ? i
  : find_button_r(w, i->next) : NULL;
}
static bool iterate_buttons(const xcb_window_t ewin,
  void (*func)(struct XSButton * restrict))
{
  struct XSButton * b = find_button_r(ewin, xstatus_head_button);
  if (b) {
    func(b);
    return true;
  }
  return false;
}
bool xstatus_toolbar_handle_expose(const xcb_window_t event_window)
{
  return iterate_buttons(event_window, xstatus_head_button->draw);
}
bool xstatus_toolbar_handle_button_press(const xcb_window_t event_window)
{
  return iterate_buttons(event_window, xstatus_head_button->cb);
}
bool xstatus_toolbar_handle_button_enter(const xcb_window_t event_window)
{
  return iterate_buttons(event_window, xstatus_head_button->enter);
}
bool xstatus_toolbar_handle_button_leave(const xcb_window_t event_window)
{
  return iterate_buttons(event_window, xstatus_head_button->leave);
}

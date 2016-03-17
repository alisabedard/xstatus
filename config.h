//Copyright 2016, Jeffrey E. Bedard <jefbed@gmail.com> 
#ifndef CONFIG_H
#define CONFIG_H

//#define FONT "-*-lucida-*-r-*-*-14-*-*-*-*-*-*-*"
//#define FONT "fixed"
#define FONT "-*-terminus-*-*-*-*-14-*-*-*-*-*-*-*"
// Adjust the height for the selected font:
#define HEIGHT 18

#define MENU "dmenu_run -b"
#define TERM "urxvt&"
#define BROWSER "qupzilla&"
#define EDITOR "urxvt -e vim&"
//#define EDITOR "gvim&"
#define MIXER "urxvt -e alsamixer&"

#define DEFAULTF "/tmp/xstatus"

#define TIMEFMT "%a %d %b %T"

#define DELAY 1
#define BORDER 1
#define PAD 4
#define BUTTON_SPACE 6
#define BUTTON_BORDER "grey25"
#define BUTTON_BG "grey2"
#define BUTTON_FG "darkgreen"
#define PANEL_BG "black"
#define PANEL_FG "green3"
#define BAT_AC	"green"
#define BAT_BAT "yellow"
#define BAT_CRIT "red"
#define CRIT_PCT 11

#define BATSYSFILE "/sys/class/power_supply/BAT0/capacity"
#define ACSYSFILE "/sys/class/power_supply/AC/online"

#endif//CONFIG_H

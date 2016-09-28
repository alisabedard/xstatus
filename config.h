// Copyright 2016, Jeffrey E. Bedard

#ifndef XS_CONFIG_H
#define XS_CONFIG_H

// Note: Adjust XS_HEIGHT for the selected font:

enum {
	XS_BORDER = 1,
	XS_BUF_SZ = 80,
	XS_CRIT_PCT = 10,
	XS_HEIGHT = 18,
	XS_PAD = 4,
	XS_WPAD = 8
};

#define FONT "-*-terminus-*-*-*-*-14-*-*-*-*-*-*-*"

#define XS_MENU "dmenu_run -b&"
#define TCMD "xterm"
#define TERM TCMD "&"
#define BROWSER "firefox&"
#define EDITOR TCMD " -e vim&"
#define MIXER TCMD " -e alsamixer&"
#define LOCK "slock&"
#define DEFAULTF "/tmp/xstatus"
#define TIMEFMT "%a %d %b %T"

#define BUTTON_BG "grey10"
#define BUTTON_FG "green4"
#define PANEL_FG "green3"
#define GC_BG_COLOR "black"
#define PANEL_BG GC_BG_COLOR
#define GC_AC_COLOR "green"
#define GC_BAT_COLOR "yellow"
#define GC_CRIT_COLOR "red"

#define BATSYSFILE "/sys/class/power_supply/BAT0/capacity"
#define ACSYSFILE "/sys/class/power_supply/AC/online"
#define TEMPSYSFILE "/sys/devices/virtual/thermal/thermal_zone0/temp"

#endif//!XS_CONFIG_H

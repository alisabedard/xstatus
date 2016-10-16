// Copyright 2016, Jeffrey E. Bedard

#ifndef XS_CONFIG_H
#define XS_CONFIG_H

// Note: Adjust XSTATUS_CONST_HEIGHT for the selected font:

enum {
	XSTATUS_CONST_BORDER = 1,
	XSTATUS_CONST_BUFFER_SIZE = 80,
	XSTATUS_CONST_CRITICAL_PERCENT = 10,
	XSTATUS_CONST_HEIGHT = 18,
	XSTATUS_CONST_PAD = 4,
	XSTATUS_CONST_WIDE_PAD = 8
};

#define XSTATUS_FONT "-*-terminus-*-*-*-*-14-*-*-*-*-*-*-*"

#define XSTATUS_MENU_COMMAND "dmenu_run -b&"
#define TCMD "xterm"
#define XSTATUS_TERMINAL TCMD "&"
#define XSTATUS_BROWSER_COMMAND "firefox&"
#define XSTATUS_EDITOR_COMMAND TCMD " -e vim&"
#define XSTATUS_MIXER_COMMAND TCMD " -e alsamixer&"
#define XSTATUS_LOCK_COMMAND "slock&"
#define XSTATUS_STATUS_FILE "/tmp/xstatus"
#define XSTATUS_TIME_FORMAT "%a %d %b %T"

#define XSTATUS_BUTTON_BG "grey10"
#define XSTATUS_BUTTON_FG "green4"

#define XSTATUS_PANEL_FOREGROUND "green3"
#define XSTATUS_PANEL_BACKGROUND "black"

#define XSTATUS_BATTERY_BACKGROUND_COLOR "black"
#define XSTATUS_BATTERY_AC_COLOR "green"
#define XSTATUS_BATTERY_BATTERY_COLOR "yellow"
#define XSTATUS_BATTERY_CRITICAL_COLOR "red"

#define XSTATUS_SYSFILE_BATTERY "/sys/class/power_supply/BAT0/capacity"
#define XSTATUS_SYSFILE_AC "/sys/class/power_supply/AC/online"
#define XSTATUS_SYSFILE_TEMPERATURE "/sys/devices/virtual/thermal/thermal_zone0/temp"

#endif//!XS_CONFIG_H

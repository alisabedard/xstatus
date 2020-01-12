// Copyright 2017, Jeffrey E. Bedard
#ifndef XSTATUS_CONFIG_H
#define XSTATUS_CONFIG_H
// Note: Adjust XSTATUS_CONST_HEIGHT for the selected font:
enum {
	XSTATUS_CONST_BORDER = 1,
	XSTATUS_CONST_BUFFER_SIZE = 80,
	XSTATUS_CONST_CRITICAL_PERCENT = 10,
	XSTATUS_CONST_HEIGHT = 18,
	XSTATUS_CONST_PAD = 4,
	XSTATUS_TIME_BUFFER_SIZE = 20
};
// Comment out the following line to enable all toolbar buttons:
//#define XSTATUS_MINIMAL_TOOLBAR
#define XSTATUS_FONT "9x15"
// Comment out a command to disable its corresponding button:
//#define XSTATUS_FM_COMMAND "thunar"
//#define XSTATUS_TERMINAL_COMMAND "xterm"
//#define XSTATUS_TERMINAL_COMMAND "jbxvt"
#define XSTATUS_BROWSER_COMMAND "firefox"
#define XSTATUS_EDITOR_COMMAND "gvim"
#define XSTATUS_MIXER_COMMAND "xterm -e alsamixer"
//#define XSTATUS_MIXER_COMMAND "pavucontrol"
//#define XSTATUS_LOCK_COMMAND "slock"
//#define XSTATUS_MAIL_COMMAND "thunderbird"
/* Set the file from which you wish to read message lines.
 * The display starts with the top line of the file.  */
// Comment out XSTATUS_STATUS_FILE to disable this feature
#define XSTATUS_STATUS_FILE "/tmp/xstatus"
#define XSTATUS_TIME_FORMAT "%a %d %b %T"
#define XSTATUS_BUTTON_BG "grey10"
#define XSTATUS_BUTTON_FG "green4"
#define XSTATUS_PANEL_FOREGROUND "green3"
#define XSTATUS_PANEL_BACKGROUND "black"
/*------------------------------*/
/* Note:  initialize_gcs() in battery.c depends on the following naming
 * conventions for its macro-based mapping of foreground colors to gcs.  */
#define XSTATUS_BATTERY_BACKGROUND_COLOR "black"
#define XSTATUS_BATTERY_AC_COLOR "green"
#define XSTATUS_BATTERY_BATTERY_COLOR "yellow"
#define XSTATUS_BATTERY_CRITICAL_COLOR "red"
/*------------------------------*/
#define XSTATUS_SYSFILE_BATTERY "/sys/class/power_supply/BAT0/capacity"
#define XSTATUS_SYSFILE_AC "/sys/class/power_supply/AC/online"
#define XSTATUS_SYSFILE_TEMPERATURE \
	"/sys/devices/virtual/thermal/thermal_zone0/temp"
#endif//!XSTATUS_CONFIG_H

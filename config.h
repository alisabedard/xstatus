// Copyright 2016, Jeffrey E. Bedard

#ifndef CONFIG_H
#define CONFIG_H

#define FONT "-*-terminus-*-*-*-*-14-*-*-*-*-*-*-*"
// Adjust the height for the selected font:
#define HEIGHT 18

#define MENU "dmenu_run -b"
#define TCMD "xterm"
#define TERM TCMD "&"
#define BROWSER "qupzilla&"
#define EDITOR TCMD " -e vim&"
#define MIXER TCMD " -e alsamixer&"
#define LOCK "slock"

#define DEFAULTF "/tmp/xstatus"

#define TIMEFMT "%a %d %b %T"

#define BORDER 1
#define PAD 4
#define BUTTON_BG "grey10"
#define BUTTON_FG "green4"
#define PANEL_BG "black"
#define PANEL_FG "green3"
#define GOOD	"green"
#define DEGRADED "yellow"
#define CRITICAL "red"
#define CRIT_PCT 11

#define BATSYSFILE "/sys/class/power_supply/BAT0/capacity"
#define ACSYSFILE "/sys/class/power_supply/AC/online"
#define TEMPSYSFILE "/sys/devices/virtual/thermal/thermal_zone0/temp"

#endif//CONFIG_H

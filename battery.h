#ifndef BATTERY_H
#define BATTERY_H

extern GC bat_bat_gc, bat_ac_gc, bat_crit_gc;

void draw_battery(Display *d, const Window w);

#endif//!BATTERY_H

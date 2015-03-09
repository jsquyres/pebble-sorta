//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#ifndef _MAIN_H_
#define _MAIN_H_

#include <pebble.h>


extern Window *s_main_window;
extern TextLayer *s_sorta_time_layer;
extern GFont s_sorta_time_font;
extern TextLayer *s_exact_time_layer;
extern GFont s_exact_time_font;
extern TextLayer *s_date_layer;
extern GFont s_date_font;
extern TextLayer *s_battery_layer;
extern GFont s_battery_font;

extern const int margin_offset;


void sorta_init(void);
void sorta_finalize(void);

void sorta_battery_handler(BatteryChargeState charge);

void sorta_update_sorta_time(struct tm *tm);
void sorta_update_exact_time(struct tm *tm);

void sorta_update_sorta_date(struct tm *tm);
void sorta_update_exact_date(struct tm *tm);

#endif

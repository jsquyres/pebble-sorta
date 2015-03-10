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
extern TextLayer *s_battery_charge_layer;
extern GFont s_battery_charge_font;
extern GBitmap *s_battery_icon;
extern BitmapLayer *s_battery_icon_layer;

extern const int margin_offset;
extern const int sorta_persistent_abi;
extern bool sorta_enable_shake_exact;
extern uint32_t sorta_shake_exact_timeout;


void sorta_init(void);
void sorta_finalize(void);
void sorta_update_shake(void);

void sorta_battery_handler(BatteryChargeState charge);

void sorta_update_sorta_time(struct tm *tm);
void sorta_update_exact_time(struct tm *tm);

void sorta_update_sorta_date(struct tm *tm);
void sorta_update_exact_date(struct tm *tm);

void sorta_persist_load(void);
void sorta_persist_save(void);

void sorta_inbox_received_callback(DictionaryIterator *iterator,
                                   void *context);
 void sorta_inbox_dropped_callback(AppMessageResult reason,
                                   void *context);
#endif

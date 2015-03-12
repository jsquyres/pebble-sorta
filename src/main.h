//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#ifndef _MAIN_H_
#define _MAIN_H_

#include <pebble.h>


// main.c
extern const int margin_offset;

// persist.c
extern bool sorta_shake_enable;
extern uint32_t sorta_shake_timeout;

typedef enum {
    SORTA_DISPLAY_MODE_SORTA,
    SORTA_DISPLAY_MODE_EXACT,
    SORTA_DISPLAY_MODE_MAX
} sorta_display_mode_t;

void sorta_init(void);
void sorta_finalize(void);
void sorta_display(void);

void sorta_battery_window_load(Window *window);
void sorta_battery_window_unload(Window *window);
void sorta_battery_init(void);
void sorta_battery_finalize(void);

void sorta_time_window_load(Window *window);
void sorta_time_window_unload(Window *window);
void sorta_time_display(struct tm *tm, sorta_display_mode_t mode);

void sorta_date_window_load(Window *window);
void sorta_date_window_unload(Window *window);
void sorta_date_display(struct tm *tm, sorta_display_mode_t mode);

void sorta_persist_load(void);
void sorta_persist_save(void);

void sorta_shake_init(void);
void sorta_shake_update(void);
sorta_display_mode_t sorta_shake_get_display_mode(void);
void sorta_shake_finalize(void);

void sorta_inbox_received_callback(DictionaryIterator *iterator,
                                   void *context);
 void sorta_inbox_dropped_callback(AppMessageResult reason,
                                   void *context);

#endif

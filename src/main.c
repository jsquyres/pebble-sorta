//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

// Global values
const int margin_offset = 5;

// Local values
static Window *s_main_window = NULL;



//
// Called from many places; this routine displays the current date/time
//
void sorta_display(void) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    // Find out if we're supposed to display sorta time or exact time
    sorta_display_mode_t mode = sorta_shake_get_display_mode();

    sorta_date_display(tm, mode);
    sorta_time_display(tm, mode);
}

/*************************************************************************/

//
// Handler for the tick service
//
static void sorta_tick_handler(struct tm *tick_time,
                               TimeUnits units_changed) {
    sorta_display();
}

/*************************************************************************/

static void window_load(Window *window) {
    window_set_background_color(window, GColorWhite);

    sorta_battery_window_load(window);
    sorta_date_window_load(window);
    sorta_time_window_load(window);
}

static void window_unload(Window *window) {
    sorta_time_window_unload(window);
    sorta_date_window_unload(window);
    sorta_battery_window_unload(window);
}

/**********************************************************************/

void sorta_init(void) {
    // Main window
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
          .load = window_load,
          .unload = window_unload,
    });
    window_stack_push(s_main_window, false);

    // Read in the persistent config values
    sorta_persist_load();

    // Register with the accelerometer/tap (shake) service (or not)
    sorta_shake_init();

    // Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, sorta_tick_handler);

    // Register with BatteryStateservice
    sorta_battery_init();

    // Register to receive configuration messages
    app_message_register_inbox_received(sorta_inbox_received_callback);
    app_message_register_inbox_dropped(sorta_inbox_dropped_callback);
    app_message_open(app_message_inbox_size_maximum(), 0);

    // Display everything
    sorta_display();
}

void sorta_finalize(void) {
    // Unsubscribe from the tick service
    tick_timer_service_unsubscribe();

    // Finalize the subsystems
    sorta_shake_finalize();
    sorta_battery_finalize();

    // Save config values
    sorta_persist_save();

    // Destroy the window
    window_destroy(s_main_window);
}

/**********************************************************************/

int main(void) {
    sorta_init();
    app_event_loop();
    sorta_finalize();
}

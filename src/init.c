//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

// Local behavior values
static int display_sorta = true;
static time_t display_exact_until = 0;

/**********************************************************************/

static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    window_set_background_color(window, GColorWhite);

    // Battery: top right corner
    int width = 30;
    int height = 14;
    int x = bounds.size.w - width;
    int y = 0;
    s_battery_layer = text_layer_create(GRect(x, y, width, height));
    s_battery_font =
        fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BATTERY_14));
    text_layer_set_font(s_battery_layer, s_battery_font);
    text_layer_set_text_alignment(s_battery_layer, GTextAlignmentLeft);
    text_layer_set_background_color(s_battery_layer, GColorClear);
    text_layer_set_text_color(s_battery_layer, GColorBlack);
    layer_add_child(window_layer,
                    text_layer_get_layer(s_battery_layer));


    // Date: bottom middle
    width = bounds.size.w - margin_offset * 2;
    height = 20;
    x = margin_offset;
    y = bounds.size.h - height - margin_offset;
    s_date_layer = text_layer_create((GRect) {
            .origin = {
                .x = x,
                .y = y
            },
            .size = {
                .w = width,
                .h = height
            }
        });
    s_date_font =
        fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DATE_14));
    text_layer_set_font(s_date_layer, s_date_font);
    text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
    text_layer_set_background_color(s_date_layer, GColorClear);
    text_layer_set_text_color(s_date_layer, GColorBlack);
    layer_add_child(window_layer, text_layer_get_layer(s_date_layer));

    // Exact time: middle
    width = bounds.size.w - margin_offset * 2;
    height = 50;
    x = margin_offset;
    y = 50;
    s_exact_time_layer = text_layer_create((GRect) {
            .origin = {
                .x = x,
                .y = y
            },
            .size = {
                .w = width,
                .h = height
            }
        });
    s_exact_time_font =
        fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TIME_34));
    text_layer_set_font(s_exact_time_layer, s_exact_time_font);
    text_layer_set_text_alignment(s_exact_time_layer, GTextAlignmentCenter);
    text_layer_set_background_color(s_exact_time_layer, GColorClear);
    text_layer_set_text_color(s_exact_time_layer, GColorBlack);
    layer_add_child(window_layer, text_layer_get_layer(s_exact_time_layer));

    // Sorta time: middle
    width = bounds.size.w - margin_offset * 2;
    height = 130;
    x = margin_offset;
    y = 20;
    s_sorta_time_layer = text_layer_create((GRect) {
            .origin = {
                .x = x,
                .y = y
            },
            .size = {
                .w = width,
                .h = height
            }
        });
    s_sorta_time_font =
        fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TIME_26));
    text_layer_set_font(s_sorta_time_layer, s_sorta_time_font);
    text_layer_set_text_alignment(s_sorta_time_layer, GTextAlignmentLeft);
    text_layer_set_background_color(s_sorta_time_layer, GColorClear);
    text_layer_set_text_color(s_sorta_time_layer, GColorBlack);
    layer_add_child(window_layer, text_layer_get_layer(s_sorta_time_layer));
}

static void window_unload(Window *window) {
    text_layer_destroy(s_sorta_time_layer);
    fonts_unload_custom_font(s_sorta_time_font);
    text_layer_destroy(s_exact_time_layer);
    fonts_unload_custom_font(s_exact_time_font);
    text_layer_destroy(s_date_layer);
    fonts_unload_custom_font(s_date_font);
    text_layer_destroy(s_battery_layer);
    fonts_unload_custom_font(s_battery_font);
}

/**********************************************************************/

static void sorta_update(bool reset) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    // Do we need to flip back from exact to sorta time display?
    if (0 != display_exact_until &&
        time(NULL) >= display_exact_until) {
        display_sorta = true;
        display_exact_until = 0;
        wakeup_cancel_all();
    }

    // NOTE: if reset==true, that means we need to clear out the time
    // text layer that is not being used

    if (display_sorta) {
        // Display the fuzzy time
        if (reset) {
            text_layer_set_text(s_exact_time_layer, "");
        }
        sorta_update_sorta_time(tm);
        sorta_update_sorta_date(tm);
    } else {
        // Display the exact time
        if (reset) {
            text_layer_set_text(s_sorta_time_layer, "");
        }
        sorta_update_exact_time(tm);
        sorta_update_exact_date(tm);
    }
}

/*************************************************************************/

//
// Handler for the tick service
//
static void sorta_tick_handler(struct tm *tick_time,
                               TimeUnits units_changed) {
    sorta_update(false);
}

/*************************************************************************/

void sorta_wakeup_restore_sorta_time() {
    display_exact_until = 1;
    sorta_update(true);
}

//
// Handler for the tap (shake) service
//
static void sorta_tap_handler(AccelAxisType axis, int32_t direction) {
    // We don't care what tap/shake happened -- just that it happened.
    display_sorta = !display_sorta;

    // If we are now displaying exact, set a timeout for returning to
    // fuzzy time.  Do it two ways:
    //
    // 1. Set a wakeup timer for 5 seconds from now.  PebbleOS sets
    // restrictions on wakeup timers, however, so try starting 5
    // seconds from now, up to two minutes from now.  If we can't set
    // a wakeup in that time, we'll end up falling over to method #2.
    //
    // 2. If the next minute tick handler fires after 15 seconds after
    // we went to exact time, it will reset back to fuzzy time.

    if (!display_sorta) {
        wakeup_service_subscribe(sorta_wakeup_restore_sorta_time);

        time_t t = time(NULL);
        display_exact_until = t + sorta_shake_exact_timeout;

        for (int i = sorta_shake_exact_timeout; i < 120; ++i) {
            int ret;
            ret = wakeup_schedule(t + i, 0, false);
            if (0 == ret) {
                break;
            }
        }
    }

    sorta_update(true);
}

/*************************************************************************/

void sorta_update_shake(void) {
    if (sorta_enable_shake_exact) {
        // Register with the Tap (shake) service
        accel_tap_service_subscribe(sorta_tap_handler);
    } else {
        accel_tap_service_unsubscribe();
    }
}

/*************************************************************************/

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
    sorta_update_shake();

    // Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, sorta_tick_handler);
    sorta_update(true);

    // Register with BatteryStateservice
    sorta_battery_handler(battery_state_service_peek());
    battery_state_service_subscribe(sorta_battery_handler);

    // Register to receive configuration messages
    app_message_register_inbox_received(sorta_inbox_received_callback);
    app_message_register_inbox_dropped(sorta_inbox_dropped_callback);

    app_message_open(app_message_inbox_size_maximum(), 0);
}

void sorta_finalize(void) {
    // Save config values
    sorta_persist_save();

    // Unsubscribe from everything
    accel_tap_service_unsubscribe();
    battery_state_service_unsubscribe();
    tick_timer_service_unsubscribe();

    // Destroy the window
    window_destroy(s_main_window);
}

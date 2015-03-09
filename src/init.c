//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

// Global behavior values
static int sorta = true;

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
        fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TIME_38));
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
    time_t temp = time(NULL);
    struct tm *tm = localtime(&temp);

    // NOTE: if reset==true, that means we need to clear out the time
    // text layer that is not being used

    if (sorta) {
        if (reset) {
            text_layer_set_text(s_exact_time_layer, "");
        }
        sorta_update_sorta_time(tm);
        sorta_update_sorta_date(tm);
    } else {
        if (reset) {
            text_layer_set_text(s_sorta_time_layer, "");
        }
        sorta_update_exact_time(tm);
        sorta_update_exact_date(tm);
    }
}

/*************************************************************************/

/*
 * Handler for the tick service
 */
static void sorta_tick_handler(struct tm *tick_time,
                               TimeUnits units_changed) {
    sorta_update(false);
}

/*************************************************************************/

static void flip(void) {
    sorta = !sorta;
    sorta_update(true);
}

/*
 * Handler for the tap service
 * (doesn't work very well :-( )
 */
static void sorta_tap_handler(AccelAxisType axis, int32_t direction) {
    flip();
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

    // Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, sorta_tick_handler);
    sorta_update(true);

    // Register with BatteryStateservice
    sorta_battery_handler(battery_state_service_peek());
    battery_state_service_subscribe(sorta_battery_handler);

    // Register with the Tap service
    accel_tap_service_subscribe(sorta_tap_handler);
}

void sorta_finalize(void) {
    accel_tap_service_unsubscribe();
    battery_state_service_unsubscribe();
    tick_timer_service_unsubscribe();

    window_destroy(s_main_window);
}

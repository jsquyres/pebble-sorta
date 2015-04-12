//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

#include <string.h>


static TextLayer *s_sorta_time_layer;
static GFont s_sorta_time_font;
static TextLayer *s_exact_time_layer;
static GFont s_exact_time_font;

static bool sorta_vals_initialized = false;
static int sorta_minute_min = 61;
static int sorta_minute_max = -61;
static char sorta_time_str[128];
static char exact_time_str[16];

static char *hour_names[] = {
    "twelve",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
    "ten",
    "eleven",
    NULL
};

static char *minute_names[] = {
    "o'clock",
    "fifteen",
    "thirty",
    "forty-five",
    NULL
};

struct day_part {
    char *name;
    int hour_start;
    int hour_end;
};

static struct day_part day_parts[] = {
    { .hour_start = 0,  .hour_end = 11, .name = "in the morning" },
    { .hour_start = 12, .hour_end = 17, .name = "in the afternoon" },
    { .hour_start = 18, .hour_end = 20, .name = "in the evening" },
    { .hour_start = 21, .hour_end = 23, .name = "at night" },
    { .name = NULL }
};

struct sorta_name {
    char *name;
    int minute_start;
    int minute_end;
};

static struct sorta_name sorta_names[] = {
    { .minute_start = -7, .minute_end = -6, .name = "around " },
    { .minute_start = -5, .minute_end = -4, .name = "about " },
    { .minute_start = -3, .minute_end = -1, .name = "nearly " },
    { .minute_start = 0,  .minute_end = 0,  .name = "" },
    { .minute_start = 1,  .minute_end = 3,  .name = "just after " },
    { .minute_start = 4,  .minute_end = 5,  .name = "a little after " },
    { .minute_start = 6,  .minute_end = 7,  .name = "after " },
    { .name = NULL }
};


// This is a separate subroutine so that it can be invoked from
// multiple places
void sorta_time_set_text_color(void) {
    text_layer_set_text_color(s_exact_time_layer, sorta_text_color);
    text_layer_set_text_color(s_sorta_time_layer, sorta_text_color);
}

void sorta_time_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    // Exact time: middle
    int width = bounds.size.w - margin_offset * 2;
    int height = 50;
    int x = margin_offset;
    int y = 50;
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
    layer_set_hidden(text_layer_get_layer(s_exact_time_layer), true);

    // Add the text layer
    layer_add_child(window_layer, text_layer_get_layer(s_exact_time_layer));

    // Sorta time: middle
    width = bounds.size.w - margin_offset * 2;
    height = 135;
    x = margin_offset;
    y = 13;
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
    layer_set_hidden(text_layer_get_layer(s_sorta_time_layer), true);

    // Add the text layer
    layer_add_child(window_layer, text_layer_get_layer(s_sorta_time_layer));

    // Set the text color of both layers
    sorta_time_set_text_color();
}

void sorta_time_window_unload(Window *window) {
    text_layer_destroy(s_sorta_time_layer);
    fonts_unload_custom_font(s_sorta_time_font);

    text_layer_destroy(s_exact_time_layer);
    fonts_unload_custom_font(s_exact_time_font);
}

/*************************************************************************/

// Helper function for printing fuzzy times
static void init_sorta_vals(void) {
    for (int i = 0; sorta_names[i].name != NULL; ++i) {
        if (sorta_names[i].minute_start < sorta_minute_min) {
            sorta_minute_min = sorta_names[i].minute_start;
        }
        if (sorta_names[i].minute_end > sorta_minute_max) {
            sorta_minute_max = sorta_names[i].minute_end;
        }
    }

    sorta_vals_initialized = true;
}

// Print a fuzzy time
static void sorta_time_display_sorta(struct tm *tm) {
    layer_set_hidden(text_layer_get_layer(s_exact_time_layer), true);
    layer_set_hidden(text_layer_get_layer(s_sorta_time_layer), false);

    if (!sorta_vals_initialized) {
        init_sorta_vals();
    }

    // Get the "sorta" name
    int hour = tm->tm_hour;
    int real_min = tm->tm_min;
    int quarter_min = real_min / 15;
    int diff = real_min - (quarter_min * 15);

    // Adjustments: if it's less than the minimum, then go back a
    // quarter.  Likewise, if it's greater than the maximum, then go
    // ahead a quarter.
    if (diff < sorta_minute_min) {
        --quarter_min;
        diff = real_min - (quarter_min * 15);
    } else if (diff > sorta_minute_max) {
        ++quarter_min;
        diff = real_min - (quarter_min * 15);
    }

    // If we're in the 4th quarter, then it's really the 0th quarter
    // of the next hour
    if (4 == quarter_min) {
        quarter_min = 0;
        hour = (hour + 1) % 24;
    }

    char *sorta_name = NULL;
    for (int i = 0; sorta_names[i].name != NULL; ++i) {
        if (diff >= sorta_names[i].minute_start &&
            diff <= sorta_names[i].minute_end) {
            sorta_name = sorta_names[i].name;
            break;
        }
    }

    // Hour name (easy)
    char *hour_name = hour_names[hour % 12];

    // Get the minute name
    char *minute_name = minute_names[quarter_min];

    // Get the day part name
    char *day_part_name = NULL;
    for (int i = 0; day_parts[i].name != NULL; ++i) {
        if (hour >= day_parts[i].hour_start &&
            hour <= day_parts[i].hour_end) {
            day_part_name = day_parts[i].name;
            break;
        }
    }

    // Make the final time string by concatenating all the strings
    // together
    snprintf(sorta_time_str, sizeof(sorta_time_str) - 1,
             "%s%s %s %s",
             sorta_name, hour_name, minute_name, day_part_name);
    text_layer_set_text(s_sorta_time_layer, sorta_time_str);
}

// Print the exact time (simple: just use strftime())
static void sorta_time_display_exact(struct tm *tm) {
    layer_set_hidden(text_layer_get_layer(s_sorta_time_layer), true);
    layer_set_hidden(text_layer_get_layer(s_exact_time_layer), false);

    char *t = exact_time_str;
    if (clock_is_24h_style()) {
        strftime(t, sizeof(exact_time_str) - 1, "%k:%M", tm);
    } else {
        strftime(t, sizeof(exact_time_str) - 1, "%l:%M %P", tm);
    }

    // strftime() will prefix single-digit hours with a space.  Skip
    // that space if it is there.
    if (' ' == t[0]) {
        ++t;
    }

    // Setup the text buffer for the time
    text_layer_set_text(s_exact_time_layer, t);
}

void sorta_time_display(struct tm *tm, sorta_display_mode_t mode) {
    switch (mode) {
    case SORTA_DISPLAY_MODE_SORTA:
        sorta_time_display_sorta(tm);
        break;

    case SORTA_DISPLAY_MODE_EXACT:
        sorta_time_display_exact(tm);
        break;

    default:
        break;
    }
}

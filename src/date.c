//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

#include <string.h>


static TextLayer *s_date_layer = NULL;
static GFont s_date_font = { 0 };

static char date_str[32] = { '\0' };

static char *month_names[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December",
    NULL
};

struct month_part {
    char *name;
    int day_begin;
    int day_end;
};

static struct month_part month_parts[] = {
    {
        .name = "early ",
        .day_begin = 0,
        .day_end = 10
    },
    {
        .name = "mid-",
        .day_begin = 11,
        .day_end = 20
    },
    {
        .name = "late",
        .day_begin = 21,
        .day_end = 32
    },
    {
        .name = NULL
    }
};


void sorta_date_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    // Date: bottom middle
    int width = bounds.size.w - margin_offset * 2;
    int height = 20;
    int x = margin_offset;
    int y = bounds.size.h - height - margin_offset;
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

    // Set the buffer for the date string
    text_layer_set_text(s_date_layer, date_str);

    // Add the text layer
    layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
}

void sorta_date_window_unload(Window *window) {
    text_layer_destroy(s_date_layer);
    fonts_unload_custom_font(s_date_font);
}

/*************************************************************************/

// Print a fuzzy date
static void sorta_date_display_sorta(struct tm *tm) {
    date_str[sizeof(date_str) - 1] = '\0';

    char *month_part = NULL;
    for (int i = 0; month_parts[i].name != NULL; ++i) {
        if (tm->tm_mday >= month_parts[i].day_begin &&
            tm->tm_mday <= month_parts[i].day_end) {
            month_part = month_parts[i].name;
            break;
        }
    }

    snprintf(date_str, sizeof(date_str) - 1, "%s%s",
             month_part, month_names[tm->tm_mon]);
}

// Print the exact date
static void sorta_date_display_exact(struct tm *tm) {
    date_str[sizeof(date_str) - 1] = '\0';

    strftime(date_str, sizeof(date_str) - 1, "%a, %b %e, %Y", tm);
}

void sorta_date_display(struct tm *tm, sorta_display_mode_t mode) {
    switch (mode) {
    case SORTA_DISPLAY_MODE_SORTA:
        sorta_date_display_sorta(tm);
        break;

    case SORTA_DISPLAY_MODE_EXACT:
        sorta_date_display_exact(tm);
        break;

    default:
        break;
    }
}

//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

#include <string.h>


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
    {
        .name = "morning",
        .hour_start = 0,
        .hour_end = 11
    },
    {
        .name = "afternoon",
        .hour_start = 12,
        .hour_end = 17
    },
    {
        .name = "evening",
        .hour_start = 18,
        .hour_end = 20
    },
    {
        .name = "night",
        .hour_start = 21,
        .hour_end = 23
    },
    {
        .name = NULL
    }
};

static bool sorta_vals_initialized = false;
static int sorta_minute_min;
static int sorta_minute_max;

struct sorta_name {
    char *name;
    int minute_start;
    int minute_end;
};

static struct sorta_name sorta_names[] = {
    {
        .name = "around ",
        .minute_start = -7,
        .minute_end = -6
    },
    {
        .name = "about ",
        .minute_start = -5,
        .minute_end = -4
    },
    {
        .name = "nearly ",
        .minute_start = -3,
        .minute_end = -1
    },
    {
        .name = "",
        .minute_start = 0,
        .minute_end = 0
    },
    {
        .name = "just after ",
        .minute_start = 1,
        .minute_end = 3
    },
    {
        .name = "about ",
        .minute_start = 4,
        .minute_end = 5
    },
    {
        .name = "around ",
        .minute_start = 6,
        .minute_end = 7
    },
    {
        .name = NULL
    }
};

/**********************************************************************/

static char time_str[128];

// Helper function for printing fuzzy times
static void init_sorta_vals(void) {
    sorta_minute_min = 61;
    sorta_minute_max = -61;

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
void sorta_update_sorta_time(struct tm *tm) {
    memset(time_str, 0, sizeof(time_str));

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
        hour = (hour + 1) % 12;
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
    char *hour_name = hour_names[tm->tm_hour % 12];

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
    snprintf(time_str, sizeof(time_str) - 1,
             "%s%s %s in the %s",
             sorta_name, hour_name, minute_name, day_part_name);

    text_layer_set_text(s_sorta_time_layer, time_str);
}

// Print the exact time (simple: just use strftime())
void sorta_update_exact_time(struct tm *tm) {
    time_str[sizeof(time_str) - 1] = '\0';

    if (clock_is_24h_style()) {
        strftime(time_str, sizeof(time_str) - 1, "%k:%M", tm);
    } else {
        strftime(time_str, sizeof(time_str) - 1, "%l:%M %P", tm);
    }

    // strftime() will prefix single-digit hours with a space.  Skip
    // that space if it is there.
    char *t = time_str;
    if (' ' == time_str[0]) {
        ++t;
    }

    text_layer_set_text(s_exact_time_layer, t);
}

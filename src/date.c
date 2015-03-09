//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

#include <string.h>


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

/**********************************************************************/

static char date_str[128];

// Print a fuzzy date
void sorta_update_sorta_date(struct tm *tm) {
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
    text_layer_set_text(s_date_layer, date_str);
}

// Print the exact date
void sorta_update_exact_date(struct tm *tm) {
    date_str[sizeof(date_str) - 1] = '\0';

    strftime(date_str, sizeof(date_str) - 1, "%a, %b %e, %Y", tm);
    text_layer_set_text(s_date_layer, date_str);
}

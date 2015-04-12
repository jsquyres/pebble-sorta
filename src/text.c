//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

// Global values
GColor sorta_text_color;
GColor sorta_background_color;


/*************************************************************************/

void sorta_text_settings_update(bool update_callbacks) {
    if (sorta_black_text) {
        sorta_text_color = GColorBlack;
        sorta_background_color = GColorWhite;
    } else {
        sorta_text_color = GColorWhite;
        sorta_background_color = GColorBlack;
    }

    // If update_callbacks==true, then call the others and tell them
    // to update their colors (otherwise, it's too early in the
    // startup sequence and we don't have anyone to update yet).
    if (update_callbacks) {
        sorta_main_set_bg_color();
        sorta_time_set_text_color();
        sorta_date_set_text_color();
        sorta_battery_set_text_color();
    }
}

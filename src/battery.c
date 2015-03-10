//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"


void sorta_battery_handler(BatteryChargeState charge) {
    static char battery_percentage_str[8];
    battery_percentage_str[sizeof(battery_percentage_str) - 1] = '\0';

    // Minimize battery usage: only draw something if there was a
    // change:
    // 1. Percentage changed
    // 2. Plugged-in status changed
    static BatteryChargeState last_charge = { 0 };

    bool need_to_draw = false;
    if (last_charge.charge_percent != charge.charge_percent ||
        last_charge.is_plugged != charge.is_plugged) {
        need_to_draw = true;
    }
    last_charge = charge;
    if (!need_to_draw) {
        return;
    }

    // Only display the battery value if it is <= 30% or if it is
    // plugged in
    if (charge.charge_percent <= 30 || charge.is_plugged) {
        snprintf(battery_percentage_str,
                 sizeof(battery_percentage_str) - 1, "%2d%%",
                 charge.charge_percent);
    } else {
        battery_percentage_str[0] = '\0';
    }
    text_layer_set_text(s_battery_charge_layer, battery_percentage_str);

    // If we're plugged in, un-hide the battery charge icon
    if (charge.is_plugged) {
        layer_set_hidden(bitmap_layer_get_layer(s_battery_icon_layer), false);
    } else {
        layer_set_hidden(bitmap_layer_get_layer(s_battery_icon_layer), true);
    }
}

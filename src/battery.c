//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"


void sorta_battery_handler(BatteryChargeState charge) {
    static char charge_str[8];
    charge_str[0] = charge_str[sizeof(charge_str) - 1] = '\0';

    // Only draw something if there is a change
    static BatteryChargeState last_charge = { 0 };
    bool changed = false;
    if (last_charge.charge_percent != charge.charge_percent ||
        last_charge.is_plugged != charge.is_plugged) {
        changed = true;
    }
    last_charge = charge;
    if (!changed) {
        return;
    }

    // Only display the battery if it is <= 30% or if it is plugged in
    if (charge.charge_percent <= 30 ||
        charge.is_plugged) {
        snprintf(charge_str, sizeof(charge_str) - 1, "%2d%%",
                 charge.charge_percent);
    }

    text_layer_set_text(s_battery_layer, charge_str);
}

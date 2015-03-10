//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"


// Graphical elements
Window *s_main_window;
TextLayer *s_sorta_time_layer;
GFont s_sorta_time_font;
TextLayer *s_exact_time_layer;
GFont s_exact_time_font;
TextLayer *s_date_layer;
GFont s_date_font;
TextLayer *s_battery_charge_layer;
GFont s_battery_charge_font;
GBitmap *s_battery_icon;
BitmapLayer *s_battery_icon_layer;

const int margin_offset = 5;
const int sorta_persistent_abi = 1;
bool sorta_enable_shake_exact = true;
uint32_t sorta_shake_exact_timeout = 5;


int main(void) {
    sorta_init();
    app_event_loop();
    sorta_finalize();
}

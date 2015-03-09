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
TextLayer *s_battery_layer;
GFont s_battery_font;

const int margin_offset = 5;


int main(void) {
    sorta_init();
    app_event_loop();
    sorta_finalize();
}

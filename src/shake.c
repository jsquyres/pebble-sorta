//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

// When a shake is detected, this is set to be
// time(NULL)+shake_timeout -- it is the time at which we want to
// reset the display back to sorta time.
static time_t display_exact_until = 0;

// What display mode are we in?  Start in sorta mode.
static sorta_display_mode_t mode = SORTA_DISPLAY_MODE_SORTA;


//
// Called at the end of the shake timeout to reset the display back to
// sorta time
//
static void sorta_wakeup_restore_sorta_mode() {
    mode = SORTA_DISPLAY_MODE_SORTA;
    display_exact_until = 0;

    sorta_display();
}

//
// Handler for the tap (shake) service
//
static void sorta_shake_handler(AccelAxisType axis, int32_t direction) {
    // We don't care what tap/shake happened -- just that it happened.
    if (SORTA_DISPLAY_MODE_SORTA == mode) {
        mode = SORTA_DISPLAY_MODE_EXACT;
    } else {
        mode = SORTA_DISPLAY_MODE_SORTA;
    }

    // If we are now displaying exact, set a timeout for returning to
    // fuzzy time.  Do it two ways:
    //
    // 1. Set a wakeup timer for 5 seconds from now.  PebbleOS sets
    // restrictions on wakeup timers, however, so try starting 5
    // seconds from now, up to two minutes from now.  If we can't set
    // a wakeup in that time, we'll end up falling over to method #2.
    //
    // 2. If the next minute tick handler fires after <timeout>
    // seconds after we went to exact time, it will reset back to
    // sorta time.
    if (SORTA_DISPLAY_MODE_EXACT == mode) {
        wakeup_cancel_all();
        wakeup_service_subscribe(sorta_wakeup_restore_sorta_mode);

        time_t t = time(NULL);
        display_exact_until = t + sorta_shake_timeout;

        for (int i = sorta_shake_timeout; i < 120; ++i) {
            int ret;
            ret = wakeup_schedule(t + i, 0, false);
            if (0 == ret) {
                break;
            }
        }
    }

    sorta_display();
}

/*************************************************************************/

void sorta_shake_init(void) {
    if (sorta_shake_enable) {
        // Register with the Tap (shake) service
        accel_tap_service_subscribe(sorta_shake_handler);
    } else {
        accel_tap_service_unsubscribe();
    }

    sorta_display();
}

void sorta_shake_update(void) {
    sorta_shake_init();
}

sorta_display_mode_t sorta_shake_get_display_mode(void) {
    // Did we fall into method #2 from sorta_shake_handler?
    time_t t = time(NULL);
    if (display_exact_until > 0 && t > display_exact_until) {
        mode = SORTA_DISPLAY_MODE_SORTA;
        wakeup_cancel_all();
    }

    return mode;
}

void sorta_shake_finalize(void) {
    wakeup_cancel_all();
    accel_tap_service_unsubscribe();
}

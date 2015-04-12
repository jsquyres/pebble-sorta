//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

#define SORTA_PERSIST_ABI_KEY 2

#define SORTA_V2_SHAKE_ENABLE_KEY  2
#define SORTA_V2_SHAKE_TIMEOUT_KEY 3
#define SORTA_V2_BLACK_TEXT_KEY    4

#define SORTA_V1_SHAKE_ENABLE_KEY  SORTA_V2_SHAKE_ENABLE_KEY
#define SORTA_V1_SHAKE_TIMEOUT_KEY SORTA_V2_SHAKE_TIMEOUT_KEY

// Global values
bool sorta_shake_enable = true;
uint32_t sorta_shake_timeout = 5;
bool sorta_black_text = true;

// Local values
static const int sorta_persistent_abi = SORTA_PERSIST_ABI_KEY;


// v1: initial set of values stored
static void read_v1(void) {
    sorta_shake_enable = persist_read_bool(SORTA_V1_SHAKE_ENABLE_KEY);
    sorta_shake_timeout = persist_read_int(SORTA_V1_SHAKE_TIMEOUT_KEY);
}

// v2 is the same as v1, except it adds one more value: black text
static void read_v2(void) {
    // It's ok to to call read_v1() here, because we set the v1 keys
    // equivalent to their v2 keys.
    read_v1();

    sorta_black_text = persist_read_bool(SORTA_V2_BLACK_TEXT_KEY);
}

void sorta_persist_load(void) {
    // Defaults
    sorta_shake_enable = true;
    sorta_shake_timeout = 5;
    sorta_black_text = true;

    if (persist_exists(SORTA_PERSIST_ABI_KEY)) {
        int version = persist_read_int(SORTA_PERSIST_ABI_KEY);

        switch (version) {
        case 1:
            read_v1();
            break;

        case 2:
            read_v2();
            break;

        default:
            APP_LOG(APP_LOG_LEVEL_ERROR,
                    "Unknown persistent data version: %d", version);
            break;
        }
    }
}

void sorta_persist_save(void) {
    persist_write_int(SORTA_PERSIST_ABI_KEY, sorta_persistent_abi);

    // This is ABI v2
    // Write the int before the bool to optimize alignment
    persist_write_int(SORTA_V2_SHAKE_TIMEOUT_KEY, sorta_shake_timeout);
    persist_write_bool(SORTA_V2_SHAKE_ENABLE_KEY, sorta_shake_enable);
    persist_write_bool(SORTA_V2_BLACK_TEXT_KEY, sorta_black_text);

    // ABI v1 was the same as ABI v1, except it did not have the
    // BLACK_TEXT value.
}

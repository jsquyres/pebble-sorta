//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

#define SORTA_PERSIST_ABI_KEY 1

#define SORTA_V1_SHAKE_ENABLE_KEY  2
#define SORTA_V1_SHAKE_TIMEOUT_KEY 3

// Global values
bool sorta_shake_enable = true;
uint32_t sorta_shake_timeout = 5;

// Local values
static const int sorta_persistent_abi = 1;


static void read_v1(void) {
    sorta_shake_enable = persist_read_bool(SORTA_V1_SHAKE_ENABLE_KEY);
    sorta_shake_timeout = persist_read_int(SORTA_V1_SHAKE_TIMEOUT_KEY);
}

void sorta_persist_load(void) {
    // Defaults
    sorta_shake_enable = true;
    sorta_shake_timeout = 5;

    if (persist_exists(SORTA_PERSIST_ABI_KEY)) {
        int version = persist_read_int(SORTA_PERSIST_ABI_KEY);

        switch (version) {
        case 1:
            read_v1();
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

    // This is ABI v1
    // Write the int before the bool to optimize alignment
    persist_write_int(SORTA_V1_SHAKE_TIMEOUT_KEY, sorta_shake_timeout);
    persist_write_bool(SORTA_V1_SHAKE_ENABLE_KEY, sorta_shake_enable);
}

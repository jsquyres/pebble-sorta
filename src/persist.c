//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

#define SORTA_PERSIST_ABI_KEY 1

#define SORTA_V1_ENABLE_SHAKE_KEY  2
#define SORTA_V1_SHAKE_TIMEOUT_KEY 3


static void read_v1(void) {
    sorta_shake_exact_timeout = persist_read_int(SORTA_V1_SHAKE_TIMEOUT_KEY);
    sorta_enable_shake_exact = persist_read_bool(SORTA_V1_ENABLE_SHAKE_KEY);
}

void sorta_persist_load(void) {
    // Defaults
    sorta_enable_shake_exact = true;
    sorta_shake_exact_timeout = 5;

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
    persist_write_int(SORTA_V1_SHAKE_TIMEOUT_KEY, sorta_shake_exact_timeout);
    persist_write_bool(SORTA_V1_ENABLE_SHAKE_KEY, sorta_enable_shake_exact);
}

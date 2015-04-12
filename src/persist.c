//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

// ABI indicator
#define SORTA_PERSIST_ABI_KEY 1

// v1 data fields
#define SORTA_V1_SHAKE_ENABLE_KEY  2
#define SORTA_V1_SHAKE_TIMEOUT_KEY 3

// v2 blob
#define SORTA_V2_KEY 4


// Local values
static const int32_t sorta_persistent_abi = 2;
static const sorta_persist_config_t config_defaults = {
    .shake_enable = true,
    .shake_timeout = 5,
    .black_text = true
};


// Global values
sorta_persist_config_t sorta_persist_config;


// v1: initial set of values stored
static void read_v1(void) {
    sorta_persist_config.shake_enable =
        persist_read_bool(SORTA_V1_SHAKE_ENABLE_KEY);
    sorta_persist_config.shake_timeout =
        persist_read_int(SORTA_V1_SHAKE_TIMEOUT_KEY);

    // v1 did not save this value
    sorta_persist_config.black_text = true;
}

// v2 has the same data as v1 plus one more field, and in a struct
// (vs. individual fields).
static void read_v2(void) {
    persist_read_data(SORTA_V2_KEY, &sorta_persist_config,
                      sizeof(sorta_persist_config));
}

void sorta_persist_load(void) {
    // Defaults
    sorta_persist_config = config_defaults;

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

    // First, delete any v1 data that may be hanging around
    if (persist_exists(SORTA_V1_SHAKE_TIMEOUT_KEY)) {
        persist_delete(SORTA_V1_SHAKE_TIMEOUT_KEY);
    }
    if (persist_exists(SORTA_V1_SHAKE_ENABLE_KEY)) {
        persist_delete(SORTA_V1_SHAKE_ENABLE_KEY);
    }

    // Now write the v2 blob
    persist_write_data(SORTA_V2_KEY, &sorta_persist_config,
                       sizeof(sorta_persist_config));
}

//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

enum {
    KEY_ENABLE_SHAKE_EXACT = 0,
    KEY_SHAKE_EXACT_TIMEOUT = 1
};

/*************************************************************************/

void sorta_inbox_received_callback(DictionaryIterator *iterator,
                                   void *context) {
    // Read first item
    Tuple *t = dict_read_first(iterator);

    while (NULL != t) {
        switch (t->key) {
        case KEY_ENABLE_SHAKE_EXACT:
            sorta_enable_shake_exact = (bool) t->value->int32;
            break;

        case KEY_SHAKE_EXACT_TIMEOUT:
            sorta_shake_exact_timeout = t->value->uint32;
            break;

        default:
            APP_LOG(APP_LOG_LEVEL_ERROR, "AppMessage key %d not recognized",
                    (int) t->key);
            break;
        }

        t = dict_read_next(iterator);
    }

    // Now that we have config data, update the callbacks
    sorta_update_shake();
}

void sorta_inbox_dropped_callback(AppMessageResult reason,
                                  void *context) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "AppMessage dropped!");
}

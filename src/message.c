//
// Copyright (c) 2015 Jeffrey M. Squyres
// $HEADER$
//

#include "main.h"

enum {
    KEY_SHAKE_ENABLE = 0,
    KEY_SHAKE_TIMEOUT = 1,
    KEY_BLACK_TEXT = 2,
};

/*************************************************************************/

void sorta_inbox_received_callback(DictionaryIterator *iterator,
                                   void *context) {
    // Read first item
    Tuple *t = dict_read_first(iterator);

    while (NULL != t) {
        switch (t->key) {
        case KEY_SHAKE_ENABLE:
            sorta_persist_config.shake_enable = (bool) t->value->int32;
            break;

        case KEY_SHAKE_TIMEOUT:
            sorta_persist_config.shake_timeout = t->value->uint32;
            break;

        case KEY_BLACK_TEXT:
            sorta_persist_config.black_text = (bool) t->value->int32;
            break;

        default:
            APP_LOG(APP_LOG_LEVEL_ERROR, "AppMessage key %d not recognized",
                    (int) t->key);
            break;
        }

        t = dict_read_next(iterator);
    }

    // Now that we have config data, tell everyone to update based on
    // the new values
    sorta_shake_update();
    sorta_text_settings_update(true);
}

void sorta_inbox_dropped_callback(AppMessageResult reason,
                                  void *context) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "AppMessage dropped!");
}

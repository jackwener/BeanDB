//
// Created by jakevin on 2020/10/25.
//

#ifndef BEANDB_TREEINFO_H
#define BEANDB_TREEINFO_H


#include "../util/config.h"

namespace DB::Index {
    struct KVEntry {
        KeyEntry kEntry;
        ValueEntry vEntry; // tuple's max size is 67B
    };

    // used for init B+Tree
    struct OpenTableInfo {
        bool isInit;
        page_id_t root_id; // only needed when (!init)
    };

}

#endif //BEANDB_TREEINFO_H

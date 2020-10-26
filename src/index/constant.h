//
// Created by jakevin on 2020/10/26.
//

#ifndef BEANDB_CONSTANT_H
#define BEANDB_CONSTANT_

#include <atomic>


namespace DB::index {
    constexpr uint32_t MIN_KEY_SIZE = BTdegree - 1;         // 7
    constexpr uint32_t MAX_KEY_SIZE = BTNodeKeySize;        // 15
    constexpr uint32_t MIN_BRANCH_SIZE = BTdegree;          // 8
    constexpr uint32_t MAX_BRANCH_SIZE = BTNodeBranchSize;  // 16
    constexpr uint32_t MIN_LEAF_SIZE = MIN_KEY_SIZE;        // 7
    constexpr uint32_t MAX_LEAF_SIZE = MAX_KEY_SIZE;        // 15
    constexpr uint32_t KEY_MIDEIUM = MAX_KEY_SIZE >> 1;     // 7 in [0...6] [7] [8...14]

    static constexpr uint32_t
            INSERT_NOTHING = 0,
            INSERT_KV = 1,
            ERASE_NOTHING = 0,
            ERASE_KV = 1;
}


#endif //BEANDB_CONSTANT_H

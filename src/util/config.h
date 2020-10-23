//
// Created by jakevin on 2020/10/21.
//

#pragma once

#include <atomic>

namespace DB {
    typedef int32_t page_id_t; // page id type
    #define INVALID_PAGE_ID -1 // representing an invalid page id
    #define PAGE_SIZE 512     // size of a data page in byte
}


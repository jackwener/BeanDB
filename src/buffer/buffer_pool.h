//
// Created by jakevin on 2020/10/19.
//

#ifndef BEANDB_BUFFER_POOL_H
#define BEANDB_BUFFER_POOL_H

#pragma once

#include <list>
#include <mutex>
#include <unordered_map>
#include "../util/config.h"

namespace DB { class Page; }
namespace DB { class Replacer; }
namespace DB {
    class BufferPoolManager {
    public:
        BufferPoolManager(size_t pool_size)

        ~BufferPoolManager()

        Page *FetchPage(page_id_t page_id);

        bool UnpinPage(page_id_t page_id, bool is_dirty);

        bool FlushPage(page_id_t page_id);

        Page *NewPage(page_id_t &page_id);

        bool DeletePage(page_id_t page_id);

    private:
        size_t pool_size_; // number of pages in buffer pool
        Page *pages_;      // array of pages
        // DiskManager *disk_manager_;
        std::unordered_map<page_id_t, Page *> *page_table_;
        Replacer<Page *> *replacer_;   // to find an unpinned page for replacement
        std::list<Page *> *free_list_; // to find a free page for replacement
        std::mutex latch_;             // to protect shared data structure
        Page *GetVictimPage();

    };
}


#endif //BEANDB_BUFFER_POOL_H

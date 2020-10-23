//
// Created by jakevin on 2020/10/21.
//

#ifndef BEANDB_PAGE_H
#define BEANDB_PAGE_H

#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <iostream>
#include "../util/config.h"
#include "../buffer_pool/buffer_pool.h"
#include "../util/rwmutex.h"

namespace DB {
    class Page {
        friend class ::DB::tree::BTree;

        friend class ::DB::buffer::BufferPoolManager;

    public:

        Page(page_t_t, page_id_t, buffer::BufferPoolManager *, bool isInit);

        virtual ~Page();

        void ref();

        void unref();

        page_t_t get_page_t() const noexcept;

        page_id_t get_page_id() const noexcept;

        char *get_data() noexcept;

        void set_dirty() noexcept;

        bool is_dirty() noexcept;

        // called when update_data and FREE-PAGE
        // return to next_free_page_id
        page_id_t add_free_page();

        // called when
        //          1. table page is drop, TableMetaPage, and all BTree Pages // TODO:
        //          2. merge, Internal or Leaf-Value, do not forget merge in ROOT-erase
        void set_free();

        // flush to disk if dirty.
        void flush();

        void force_flush();

        // return true if the lock is acquired.
        bool try_page_read_lock();

        bool try_page_write_lock();

        void page_read_lock();

        void page_write_lock();

        void page_read_unlock();

        void page_write_unlock();

        // update the all metadata into memory, for the later `flush()`.
        virtual void update_data() = 0;

        Page(const Page &) = delete;

        Page &operator=(const Page &) = delete;

        Page(Page &&) = delete;

        Page &operator=(Page &&) = delete;


    private:
        // method used by buffer pool manager
        inline void ResetMemory() { memset(data_, 0, PAGE_SIZE); }

        // members
        char data_[PAGE_SIZE]; // actual data
        page_id_t page_id_ = INVALID_PAGE_ID;
        int pin_count_ = 0;
        bool is_dirty_ = false;
        RWMutex rwlatch_;
    }


#endif //BEANDB_PAGE_H

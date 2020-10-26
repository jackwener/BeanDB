//
// Created by jakevin on 2020/10/21.
//

#ifndef BEANDB_B_PLUS_TREE_PAGE_H
#define BEANDB_B_PLUS_TREE_PAGE_H

#include "../util/config.h"
#include "page.h"

namespace DB::page {
    // Abstract class.
    class BTreePage :  Page {
        friend class ::DB::tree::BTree;
    public:

        BTreePage(page_t_t, page_id_t, page_id_t parent_id, uint32_t nEntry, buffer::BufferPoolManager *,
                  key_t_t, uint32_t str_len, bool isInit);

        virtual ~BTreePage();

        virtual void update_data() = 0;

        page_id_t get_parent_id() const noexcept;

        uint32_t get_nEntry() const noexcept;

        void set_parent_id(page_id_t) noexcept;

        key_t_t get_key_t() const noexcept;

        uint32_t get_str_len() const noexcept;

        // insert key at `index`.
        void insert_key(uint32_t index, const KeyEntry &);

        // erase key at `index`.
        // *** the caller should later changes keys[index] and values[index] ***
        void erase_key(uint32_t index);

        // called when key_t is (VAR)CHAR
        KeyEntry read_key(uint32_t index) const;


    public:

        int32_t *keys_;    // nEntry // WTF, if in protected, BTree can not access this.
        page_id_t parent_id_;
        uint32_t nEntry_;
        const key_t_t key_t_;
        const uint32_t str_len_;

        // uint32_t last_offset_; // used for CHAR-key, insert ** from bottom to up **,
        // denotes the last front offset, initialized as `PAGE_SIZE`.
        // It means data_[last_offset_-1] == '\0' for the next key-str.
        // *** not record on disk, recovered when read this page. ***

    }; // end class BTreePage
}


#endif //BEANDB_B_PLUS_TREE_PAGE_H

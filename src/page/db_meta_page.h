//
// Created by jakevin on 2020/10/26.
//

#ifndef BEANDB_DB_META_PAGE_H
#define BEANDB_DB_META_PAGE_H


#include "page.h"

namespace DB::page {
    class DBMetaPage : public Page {
    public:

        static constexpr uint32_t TABLE_NAME_STR_BLOCK = 25;
        static constexpr uint32_t MAX_TABLE_NAME_STR = 24;
        static constexpr uint32_t MAX_TABLE_NUM = 30;

        DBMetaPage(page_id_t, buffer::BufferPoolManager *, bool isInit,
                   uint32_t cur_page_no, uint32_t table_num, page_id_t next_free_page_id);

        ~DBMetaPage();

        page_id_t find_table(const std::string &);

        bool insert_table(page_id_t, const std::string &);

        bool drop_table(const std::string &);

        virtual void update_data();

    public:
        uint32_t cur_page_no_; // out of date, update from disk_manager in update_data().
        uint32_t table_num_;
        page_id_t next_free_page_id_;
        uint32_t *table_page_ids_;
        uint32_t *table_name_offset_;
        std::unordered_map<std::string, page_id_t> table_name2id_;
    };

}


#endif //BEANDB_DB_META_PAGE_H

//
// Created by jakevin on 2020/10/25.
//

#ifndef BEANDB_TABLE_PAGE_H
#define BEANDB_TABLE_PAGE_H


#include "page.h"

namespace DB::page {
    class TableMetaPage : public Page {
        friend TableMetaPage *
        parse_TableMetaPage(buffer::BufferPoolManager *buffer_pool, const char(&buffer)[page::PAGE_SIZE]);

    public:
        static constexpr uint32_t COLUMN_NAME_STR_BLOCK = 51;
        static constexpr uint32_t MAX_COLUMN_NAME_STR = 50;
        static constexpr uint32_t MAX_COLUMN_NUM = 15;
        static constexpr uint32_t NOT_A_COLUMN = MAX_COLUMN_NUM + 1;

        // if no PK, the column `autoPK` will be added at the first column
        // with type INTEGER, and will get auto increased value when INSERT.
        //
        //

        TableMetaPage(buffer::BufferPoolManager *buffer_pool, page_id_t,
                      bool isInit, key_t_t key_t, uint32_t str_len,
                // below 3 are needed only when (!init)
                      page_id_t BT_root_id, uint32_t col_num, page_id_t default_value_page_id);

        ~TableMetaPage() override;

        bool is_PK(const std::string &col_name) const;

        bool is_FK(const std::string &col_name) const;

        bool is_not_null(const std::string &col_name) const;

        bool is_default_col(const std::string &col_name) const;

        ValueEntry get_default_value(const std::string &col_name) const;

        void insert_default_value(ColumnInfo *, const std::string &default_value);

        void insert_default_value(ColumnInfo *, int32_t default_value);

        bool hasPK() const;

        key_t_t PK_t() const;

        page_id_t get_auto_id();

        range_t get_col_range(const std::string &);

        // only used when creating table
        void insert_column(const std::string &, ColumnInfo *);

        virtual void update_data();

        void set_dirty_on_insert_or_delete();

    public:
        page_id_t BT_root_id_;
        uint32_t col_num_;
        page_id_t default_value_page_id_ = NOT_A_PAGE;
        std::atomic<page_id_t> auto_id_ = NOT_A_PAGE;
        ValuePage *value_page_;
        std::unordered_map<std::string, ColumnInfo *> col_name2col_;
        uint32_t pk_col_ = NOT_A_COLUMN;                // `NOT_A_COLUMN` denotes no PK
        std::vector<std::string> cols_;
        tree::BTree *bt_;
    };}


#endif //BEANDB_TABLE_PAGE_H

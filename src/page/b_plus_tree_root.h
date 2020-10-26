//
// Created by jakevin on 2020/10/21.
//

#ifndef BEANDB_B_PLUS_TREE_ROOT_H
#define BEANDB_B_PLUS_TREE_ROOT_H


namespace DB::page{
    class RootPage : public InternalPage {
        friend class ::DB::tree::BTree;

        friend class BTreePage;

        friend RootPage *parse_RootPage(buffer::BufferPoolManager *buffer_pool, const char(&buffer)[page::PAGE_SIZE]);

    public:
        RootPage(buffer::BufferPoolManager *, page_t_t, page_id_t parent_id, page_id_t,
                 uint32_t nEntry, key_t_t, uint32_t str_len, page_id_t value_page_id, bool isInit);

        virtual ~RootPage();

        void change_to_leaf(buffer::BufferPoolManager *buffer_pool);

        void change_to_internal(buffer::BufferPoolManager *buffer_pool);

        // read the value record into ValueEntry
        void read_value(uint32_t index, ValueEntry &) const;

        // insert value at `index`.
        void insert_value(uint32_t index, const ValueEntry &);

        // erase value-str in value page corrsponding to keys[index].
        void erase_value(uint32_t index);

        // update value at `index`.
        void update_value(uint32_t index, const ValueEntry &);

        void set_left_leaf(page_id_t);

        void set_right_leaf(page_id_t);

        page_id_t get_left_leaf() const;

        page_id_t get_right_leaf() const;

        // update the all metadata into memory, for the later `flush()`.
        virtual void update_data();

    public:
        // for ROOT_LEAF
        ValuePage *value_page_;
        page_id_t value_page_id_;
        uint32_t *values_; // points to offset of the value-blocks.
        page_id_t previous_page_id_, next_page_id_;

    };
}

#endif //BEANDB_B_PLUS_TREE_ROOT_H

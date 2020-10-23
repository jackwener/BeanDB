//
// Created by jakevin on 2020/10/21.
//

#ifndef BEANDB_B_PLUS_TREE_LEAF_H
#define BEANDB_B_PLUS_TREE_LEAF_H


namespace DB {
    class LeafPage : public BTreePage {
        friend class ::DB::tree::BTree;

        friend class BTreePage;

    public:
        LeafPage(buffer::BufferPoolManager *, page_id_t, page_id_t parent_id, uint32_t nEntry,
                 key_t_t, uint32_t str_len, page_id_t value_page_id, bool isInit);

        virtual ~LeafPage();

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
        ValuePage *value_page_;
        page_id_t value_page_id_;
        page_id_t previous_page_id_, next_page_id_;
        uint32_t *values_; // points to offset of the value-blocks.
    };
}


#endif //BEANDB_B_PLUS_TREE_LEAF_H

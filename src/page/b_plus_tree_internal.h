//
// Created by jakevin on 2020/10/21.
//

#ifndef BEANDB_B_PLUS_TREE_INTERNAL_H
#define BEANDB_B_PLUS_TREE_INTERNAL_H


namespace DB::page{
    // for INTERNAL
    class InternalPage : public BTreePage {
        friend class ::DB::tree::BTree;

    public:
        InternalPage(page_t_t, page_id_t, page_id_t parent_id, uint32_t nEntry,
                     buffer::BufferPoolManager *, key_t_t, uint32_t str_len, bool isInit);

        virtual ~InternalPage();

        // update the all metadata into memory, for the later `flush()`.
        virtual void update_data();

    public:
        page_id_t *branch_;     // nEntry + 1

    }; // end class InternalPage
}


#endif //BEANDB_B_PLUS_TREE_INTERNAL_H

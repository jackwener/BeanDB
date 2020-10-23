//
// Created by jakevin on 2020/10/21.
//

#ifndef BEANDB_B_PLUS_TREE_PAGE_H
#define BEANDB_B_PLUS_TREE_PAGE_H

#include "../util/config.h"

namespace DB { class Page; }
namespace DB {
    // Abstract class.
    class BPlusTreePage {
    public:
        bool IsLeafPage() const;

        bool IsRootPage() const;

        void SetPageType(IndexPageType page_type);

        int GetSize() const;

        void SetSize(int size);

        void IncreaseSize(int amount);

        int GetMaxSize() const;

        void SetMaxSize(int max_size);

        int GetMinSize() const;

        page_id_t GetParentPageId() const;

        void SetParentPageId(page_id_t parent_page_id);

        page_id_t GetPageId() const;

        void SetPageId(page_id_t page_id);

        void SetLSN(lsn_t lsn = INVALID_LSN);

    private:
        // member variable, attributes that both internal and leaf page share
        IndexPageType page_type_;
        lsn_t lsn_;
        int size_;
        int max_size_;
        page_id_t parent_page_id_;
        page_id_t page_id_;
    };
}


#endif //BEANDB_B_PLUS_TREE_PAGE_H

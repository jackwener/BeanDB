//
// Created by jakevin on 2020/10/19.
//

#ifndef BEANDB_INDEX_ITERATOR_H
#define BEANDB_INDEX_ITERATOR_H

#include <atomic>


class BTit {
public:
    BTit(buffer::BufferPoolManager *, BTreePage *leaf, uint32_t cur_index);

    BTit(const BTit &);

    void operator=(const BTit &);

    ~BTit();

    void operator++();                  // programmer promise the state before ++ is valid.
    bool operator!=(const BTit &) const;

    // [depricated] void destroy();     // destroy the iterator when not use
    KeyEntry getK() const;

    ValueEntry getV() const;

    void updateV(const ValueEntry &);

private:
    BTreePage *leaf_;
    uint32_t cur_index_;
    buffer::BufferPoolManager *buffer_pool_;
};


#endif //BEANDB_INDEX_ITERATOR_H

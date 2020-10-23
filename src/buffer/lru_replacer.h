//
// Created by jakevin on 2020/10/21.
//

#ifndef BEANDB_LRU_REPLACER_H
#define BEANDB_LRU_REPLACER_H

#include <unordered_map>
#include <list>
#include <mutex>
#include "replacer.h"

namespace DB {
    template<typename T>
    class LRUReplacer : public Replacer<T> {
    public:
        // do not change public interface
        LRUReplacer();

        ~LRUReplacer();

        void Insert(const T &value);

        bool Victim(T &value);

        bool Erase(const T &value);

        size_t Size();

    private:
        // add your member variables here
        std::unordered_map<T, typename std::list<T>::iterator> hash;
        std::list<T> lst;
        std::mutex mtx;
    };

} // namespace cmudb


#endif //BEANDB_LRU_REPLACER_H

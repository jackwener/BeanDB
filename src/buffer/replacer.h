//
// Created by jakevin on 2020/10/21.
//

#ifndef BEANDB_REPLACER_H
#define BEANDB_REPLACER_H

#include <cstdlib>


namespace DB {
    template<typename T>
    class Replacer {
    public:
        Replacer() = default;
        virtual ~Replacer() = default;

        virtual void Insert(const T &value) = 0;

        virtual bool Victim(T &value) = 0;

        virtual bool Erase(const T &value) = 0;

        virtual size_t Size() = 0;
    };
}

#endif //BEANDB_REPLACER_H

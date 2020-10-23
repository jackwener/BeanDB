//
// Created by jakevin on 2020/10/19.
//

#ifndef BEANDB_VM_H
#define BEANDB_VM_H


namespace vm {
    class VM {
    public:
        VM()
        ~VM()

        void init();
        // run db task until user input "exit"
        void start();
    };
};


#endif //BEANDB_VM_H

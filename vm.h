#ifndef VM_H
#define VM_H
    #include "chunk.h"

    typedef struct {
        chunk* chunk;
    } VM;

    void initVM();
#endif
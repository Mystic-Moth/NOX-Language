#ifndef VM_H
#define VM_H
    #include <stack>
    #include <vector>
    #include "chunk.h"

    typedef struct {
        chunk* chunk;
        uint8_t* pc;
        std::stack<value> stack;
        std::vector<value> locals;
    } VM;
    typedef enum {
        INTERPRET_OK,
        INTERPRET_COMPILE_ERROR,
        INTERPRET_RUNTIME_ERROR
    } interpretResult;

    interpretResult interpret(chunk* chunk);

    void push(value constant);
    value pop();

    void initVM();
#endif
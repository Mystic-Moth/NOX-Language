#include "vm.h"
#include <iostream>

VM vm;

void initVM()
{
    
}
static interpretResult run()
{
    #define READ_BYTE() (*vm.pc++)
    #define READ_CONSTANT() (vm.chunk->constants[READ_BYTE()])
    #define BINARY_OP(op) \
    do { \
      double b = pop(); \
      double a = pop(); \
      push(a op b); \
    } while (false)
    
    while (true)
    {
        uint8_t instruction;

        switch (instruction = READ_BYTE())
        {
            case OP_ADD:      BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE:   BINARY_OP(/); break;
            case OP_ECHO:
            {
                cout << pop() << '\n';
                break;
            }
            case OP_CONSTANT:
            {
                value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_NEGATE:
            {
                push(-pop());
                break;
            }
            case OP_RETURN:
            {
                return INTERPRET_OK;
            }
        }
    }
    
    #undef READ_CONSTANT
    #undef READ_BYTE
    #undef BINARY_OP
}
void push(value constant)
{
    vm.stack.push(constant);
}
value pop()
{
    value top = vm.stack.top();
    vm.stack.pop();
    return top;
}
interpretResult interpret(chunk* chunk)
{
    vm.chunk = chunk;
    vm.pc = &vm.chunk->code[0];
    for (auto a : chunk->code)
    {
        cout << "code: " << (int)a << '\n';
    }
    return run();
}
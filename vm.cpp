#include "vm.h"
#include "object.h"
#include <iostream>
#include <unordered_map>

using namespace std;

VM vm;
unordered_map<string, value> globals;

void initVM()
{
    
}
void printObject(value value)
{
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            std::cout << AS_CPPSTRING(value);
            break;
    }
}
void printStatement(value val)
{
    switch (val.type) {
        case VAL_BOOL:
          std::cout << (AS_BOOL(val) ? "true" : "false");
          break;
        case VAL_NIL: std::cout << "nil"; break;
        case VAL_NUMBER: std::cout << AS_NUMBER(val); break;
        case VAL_OBJ: printObject(val); break;
    }
    std::cout << '\n';
}
static bool isFalsey(value val) {
    return IS_NIL(val) || (IS_BOOL(val) && !AS_BOOL(val));
}
static bool valuesEqual(value a, value b) {
    if (a.type != b.type) return false;
    switch (a.type) {
        case VAL_BOOL:   return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL:    return true;
        case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_OBJ: return AS_CPPSTRING(a) == AS_CPPSTRING(b);
        default: return false; // Unreachable.
    }
}
static interpretResult run()
{
    #define READ_BYTE() (*vm.pc++)
    #define READ_CONSTANT() (vm.chunk->constants[READ_BYTE()])
    #define READ_STRING() AS_STRING(READ_CONSTANT())

    #define BINARY_OP(valueType, op) \
    do { \
            if (!IS_NUMBER(vm.stack.top())) { \
                return INTERPRET_RUNTIME_ERROR; \
            } \
        double b = AS_NUMBER(pop()); \
            if (!IS_NUMBER(vm.stack.top())) { \
                push(NUMBER_VAL(b)); \
                return INTERPRET_RUNTIME_ERROR; \
            } \
        double a = AS_NUMBER(pop()); \
        push(valueType(a op b)); \
    } while (false)
    
    while (true)
    {
        uint8_t instruction;

        switch (instruction = READ_BYTE())
        {
            case OP_ADD: 
            {
                if (IS_STRING(vm.stack.top()))
                {
                    string a = AS_CPPSTRING(pop());
                    if (IS_STRING(vm.stack.top()))
                    {
                        // cout << a << '\n';
                        string b = AS_CPPSTRING(pop());
                        // cout << b << '\n';
                        ObjString* result = copyString(b+a);
                        push(OBJ_VAL(result));
                    }
                    else
                    {
                        ObjString* result;
                        result->str = a;
                        push(OBJ_VAL(result));
                        return INTERPRET_RUNTIME_ERROR;
                    }
                }
                else
                {
                    BINARY_OP(NUMBER_VAL, +);
                }
                break;
            }     
            case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
            case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;
            case OP_DIVIDE:   BINARY_OP(NUMBER_VAL, /); break;

            case OP_NIL: push(NIL_VAL); break;
            case OP_TRUE: push(BOOL_VAL(true)); break;
            case OP_FALSE: push(BOOL_VAL(false)); break;

            case OP_NOT: push(BOOL_VAL(isFalsey(pop()))); break;

            case OP_POP: pop(); break;

            case OP_DEFINE_GLOBAL:
            {
                ObjString* name = READ_STRING();
                globals[name->str] = pop();
                break;
            }
            case OP_GET_GLOBAL:
            {
                ObjString* name = READ_STRING();
                if (globals.find(name->str) == globals.end()) // did not find global variable
                {   
                    cout << "Undefined variable '" << name->str << "'.\n";
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(globals[name->str]);
                break;
            }
            
            case OP_GREATER:        BINARY_OP(BOOL_VAL, >); break;
            case OP_LESS:           BINARY_OP(BOOL_VAL, <); break;
            case OP_GREATER_EQUAL:  BINARY_OP(BOOL_VAL, >=); break;
            case OP_LESS_EQUAL:     BINARY_OP(BOOL_VAL, <=); break;
            case OP_NOT_EQUAL: {
                value b = pop();
                value a = pop();
                push(BOOL_VAL(!valuesEqual(a, b)));
                break;
            }
            case OP_EQUAL: {
                value b = pop();
                value a = pop();
                cout << valuesEqual(a, b) << '\n';
                push(BOOL_VAL(valuesEqual(a, b)));
                break;
            }
            case OP_ECHO:
            {
                printStatement(pop());
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
                if (!IS_NUMBER(vm.stack.top())) {
                    // runtimeError("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(NUMBER_VAL(-AS_NUMBER( pop() )) );
                break;
            }
            case OP_RETURN:
            {
                return INTERPRET_OK;
            }
        }
    }
    #undef READ_CONSTANT
    #undef READ_STRING
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
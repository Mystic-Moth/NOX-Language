#include <string>

#include "debug.h"
#include "chunk.h"

#define decode(code) case code: return #code

string disassemble(int opCode)
{
    switch(opCode)
    {
        decode(OP_CONSTANT);
        decode(OP_NEGATE);
        decode(OP_ADD);
        decode(OP_SUBTRACT);
        decode(OP_DIVIDE);
        decode(OP_MULTIPLY);

        decode(OP_NIL);
        decode(OP_TRUE);
        decode(OP_FALSE);
        decode(OP_NOT);

        decode(OP_EQUAL);
        decode(OP_NOT_EQUAL);
        decode(OP_GREATER);
        decode(OP_LESS);
        decode(OP_EQUAL_EQUAL);
        decode(OP_GREATER_EQUAL);
        decode(OP_LESS_EQUAL);

        decode(OP_POP);
        decode(OP_POP_LOCAL);
        decode(OP_DEFINE_GLOBAL);
        decode(OP_GET_GLOBAL);
        decode(OP_SET_GLOBAL);
        decode(OP_DEFINE_LOCAL);
        decode(OP_SET_LOCAL);
        decode(OP_GET_LOCAL);

        decode(OP_ECHO);
        decode(OP_RETURN);
    }
    return "UNKNOWN OPCODE";
}
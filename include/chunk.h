
#ifndef CHUNK_H
#define CHUNK_H
    #include <cstdint>
    #include <vector>

    #include "value.h"

    using namespace std;

    enum opCode
    {
        OP_CONSTANT,
        OP_NEGATE,
        OP_ADD,
        OP_SUBTRACT,
        OP_DIVIDE,
        OP_MULTIPLY,
        OP_ECHO,
        OP_RETURN
    };

    typedef struct
    {
        vector<uint8_t> code;
        vector<value> constants;
        vector<int> lines;
    } chunk;

    

    void initChunk(chunk* chunk);
    void writeChunk(chunk* chunk, uint8_t byte, int line);

    int addConstant(chunk* chunk, value value);

#endif

#ifndef CHUNK_H
#define CHUNK_H
    #include "value.h"
    #include <cstdint>
    #include <vector>

    enum opCode
    {
        OP_CONSTANT,
        OP_RETURN
    };

    typedef struct
    {
        vector<uint8_t*> code;
        vector<value*> constants;
    } chunk;

    typedef struct {
        chunk* chunk;
      } VM;

    void initChunk(chunk* chunk);

    int addConstant(chunk* chunk, value value);

#endif
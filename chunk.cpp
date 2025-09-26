#include "chunk.h"

void initChunk(chunk* chunk)
{
    chunk->code = NULL;
    chunk->constants = NULL;
}
int addConstant(chunk* chunk, value value)
{
    &chunk->constants.push_back(value);
    return chunk->constants.size() - 1;
}
void writeChunk(chunk* chunk, uint8_t byte)
{
    chunk->code.push_back(byte);
}
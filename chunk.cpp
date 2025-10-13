#include "chunk.h"
#include <iostream>

int addConstant(chunk* chunk, value value)
{
    chunk->constants.push_back(value);
    return chunk->constants.size() - 1;
}
void writeChunk(chunk* chunk, uint8_t byte, int line)
{
    cout << "gonna write byte " << (int)byte << '\n';
    chunk->code.push_back(byte);
    chunk->lines.push_back(line);
    // cout << "byte: " << static_cast<int>(byte) << '\n';
}
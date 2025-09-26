#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "compiler.h"
#include "chunk.h"
#include "vm.h"

using namespace std;


string readFile(string filePath)
{
    ifstream file(filePath);

    string sourceCode;
    string tmp;

    while(getline(file, tmp)) {
        sourceCode += tmp + '\n';
        cout << tmp << '\n';
    }
    return sourceCode;
}
void runFile(char* characters)
{
    string filePath(characters);
    string sourceCode = readFile(filePath);

    chunk chunk;
    bool successful = compile(sourceCode, &chunk);

    // initVM();
}
void REPL()
{

}
void test()
{
    chunk chunk;

    int constant = addConstant(&chunk, 20);
    writeChunk(&chunk, OP_CONSTANT, 0);
    writeChunk(&chunk, constant, 0);

    constant = addConstant(&chunk, 2);
    writeChunk(&chunk, OP_CONSTANT, 0);
    writeChunk(&chunk, constant, 0);

    writeChunk(&chunk, OP_DIVIDE, 0);
    writeChunk(&chunk, OP_ECHO, 0);
    writeChunk(&chunk, OP_RETURN, 0);

    interpretResult result = interpret(&chunk);
}
int main(int argc, char *argv[]) {
    if (argc == 1)
    {
        REPL();
        return 0;
    }
    switch(*argv[1])
    {
        case '0': runFile(argv[2]); break;
        case '1': test(); break;
    }
}
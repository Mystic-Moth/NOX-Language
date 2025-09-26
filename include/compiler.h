#ifndef COMPILER_H
#define COMPILER_H

    #include "Lexer.h"
    #include "vm.h"

    bool compile(std::string source, chunk* chunk);

#endif
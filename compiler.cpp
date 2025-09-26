#include <string>
#include <iostream>
#include <vector>

#include "compiler.h"

using namespace std;

typedef struct
{
    token* current;
    token* previous;
    bool hadError;
} Parser;
Parser parser;
chunk* compilingChunk;

static chunk* currentChunk() {
  return compilingChunk;
}

bool compile(string source, chunk* chunk)
{
    generateTokens(source);
    compilingChunk = chunk;
    parser.current = &tokens[0];
    parser.hadError = false;    

    for (token t : tokens) {
        cout << t.line << " | " << "type: " << t.type << "|" << t.text << "|" << "\n";
    }
    // consume(TOKEN_EOF, "Expect end of expression.");
    return !parser.hadError;
}
static void advance()
{
    parser.previous = parser.current;

    while (true)
    {
        parser.current++;
        if (parser.current->type != TOKEN_ERROR) break;
        // loop through errors
    }
}
static void errorAt(token* token, string message)
{
    cout << "line " << token->line << " | compiler error: ";
    cout << token->text << " | " << message;
    parser.hadError = true;
}
static void errorAtCurrent(string message)
{

}
static void consume(TokenType type, string message) {
    if (parser.current->type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}
static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous->line);
}
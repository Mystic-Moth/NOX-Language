#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>

#include "compiler.h"

using namespace std;

typedef struct
{
    token* previous;
    token* current;
    bool hadError;
} Parser;

typedef enum {
    PREC_NONE,
    PREC_CALL,        // . ()
    PREC_ASSIGNMENT,  // =
    PREC_OR,          // or
    PREC_AND,         // and
    PREC_EQUALITY,    // == !=
    PREC_COMPARISON,  // < > <= >=
    PREC_TERM,        // + -
    PREC_FACTOR,      // * /
    PREC_UNARY,       // ! -
    PREC_PRIMARY
} Precedence;

unordered_map<TokenType, Precedence> infixPrecedence = {
{TOKEN_NUMBER, PREC_PRIMARY},
{TOKEN_PLUS, PREC_TERM},
{TOKEN_MINUS, PREC_TERM},
{TOKEN_STAR, PREC_FACTOR},
{TOKEN_SLASH, PREC_FACTOR},
{TOKEN_LEFT_PAREN, PREC_CALL},
{TOKEN_RIGHT_PAREN, PREC_CALL},
{TOKEN_NOT, PREC_UNARY}
};

Parser parser;
chunk* compilingChunk;

static chunk* currentChunk() {
  return compilingChunk;
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
static void consume(TokenType type, string message)
{
    if (parser.current->type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}
static void emitByte(uint8_t byte)
{
    writeChunk(currentChunk(), byte, parser.current->line);
}
static void emitBytes(uint8_t byte1, uint8_t byte2)
{
    emitByte(byte1);
    emitByte(byte2);
}
static void emitReturn() {
    emitByte(OP_RETURN);
}
static void emitConstant(value value)
{
    int constant = addConstant(currentChunk(), value);
    emitBytes(OP_CONSTANT, constant);
}
static void number()
{
    cout << "number string: " << parser.current->text <<'\n';
    double value = stod(parser.current->text);
    emitConstant(value);
}
// pratt parser to hard
// lets do shunting yard algorithm 
static TokenType peek()
{
    return parser.current->type;
}
static void compileOperation(TokenType operation)
{
    switch(operation)
    {
        case TOKEN_PLUS: emitByte(OP_ADD); cout << "add\n"; break;
        case TOKEN_MINUS: emitByte(OP_SUBTRACT); cout << "subtract\n"; break;
        case TOKEN_SLASH: emitByte(OP_DIVIDE); cout << "divide\n"; break;
        case TOKEN_STAR: emitByte(OP_MULTIPLY); cout << "multiply\n"; break;
    }
}
static void expression()
{
    #define compileTop() \
    do { \
    compileOperation(operatorStack.top()); \
    operatorStack.pop(); } \
    while (false)

    stack<TokenType> operatorStack;

    while (infixPrecedence[peek()] != PREC_NONE)
    {
        TokenType type = peek();
        cout << "type: " << type << '\n';
        cout << "stack size: " << operatorStack.size() << '\n';
        switch(type)
        {
            case TOKEN_NUMBER:
            {
                cout <<"number\n";
                number(); // push number to output
                break;
            }
            case TOKEN_LEFT_PAREN:
            {
                operatorStack.push(TOKEN_LEFT_PAREN);
                break;
            }
            case TOKEN_RIGHT_PAREN:
            {
                while (operatorStack.top() != TOKEN_LEFT_PAREN)
                {
                    cout << "compiling praen\n";
                    compileTop();
                }
                operatorStack.pop(); // pop ')'
                cout << "paren complete\n";
                break;
            }
            case TOKEN_NOT:
            {
                advance();
                number();
                emitByte(OP_NEGATE);
                break;
            }
            default: // an operator like '+'
            {
                while(!operatorStack.empty() && infixPrecedence[operatorStack.top()] > infixPrecedence[type])
                {
                    compileTop();
                }
                operatorStack.push(type);
                cout << "pushed operator: " << type << '\n';
            }
        }
        advance();
    }
    while (!operatorStack.empty())
    {
        compileTop();
    }
    cout << "expression compelte\n";
    #undef compileTop
}
bool compile(string source, chunk* chunk)
{
    generateTokens(source);
    compilingChunk = chunk;
    parser.current = &tokens[0];
    parser.hadError = false;    

    // while (parser.current != &tokens[0] + tokens.size())
    // {

    // }
    for (token t : tokens) {
        cout << t.line << " | " << "type: " << t.type << "|" << t.text << "|" << "\n";
    }
    expression();
    emitByte(OP_ECHO);
    emitByte(OP_RETURN);

    
    // consume(TOKEN_EOF, "Expect end of expression.");
    return !parser.hadError;
}
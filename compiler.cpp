#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>

#include "object.h"
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
    PREC_STRING,      // strings or indentifiers
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
{TOKEN_STRING, PREC_STRING},
{TOKEN_IDENTIFIER, PREC_STRING},

{TOKEN_NUMBER, PREC_PRIMARY},
{TOKEN_FALSE, PREC_PRIMARY},
{TOKEN_TRUE, PREC_PRIMARY},
{TOKEN_NIL, PREC_PRIMARY},

{TOKEN_PLUS, PREC_TERM},
{TOKEN_MINUS, PREC_TERM},

{TOKEN_STAR, PREC_FACTOR},
{TOKEN_SLASH, PREC_FACTOR},

{TOKEN_LEFT_PAREN, PREC_CALL},
{TOKEN_RIGHT_PAREN, PREC_CALL},

{TOKEN_EQUAL_EQUAL, PREC_EQUALITY},
{TOKEN_NOT_EQUAL, PREC_EQUALITY},
{TOKEN_GREATER, PREC_COMPARISON},
{TOKEN_LESS, PREC_COMPARISON},
{TOKEN_GREATER_EQUAL, PREC_COMPARISON},
{TOKEN_LESS_EQUAL, PREC_COMPARISON},

{TOKEN_NOT, PREC_UNARY},
};

Parser parser = {nullptr, nullptr, false};;
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
    double valuee = stod(parser.current->text);
    emitConstant( NUMBER_VAL(valuee) );
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

        case TOKEN_EQUAL_EQUAL: emitByte(OP_EQUAL); cout << "equal\n"; break;
        case TOKEN_GREATER: emitByte(OP_GREATER); cout << "greater\n"; break;
        case TOKEN_LESS: emitByte(OP_LESS); cout << "less\n"; break;
        case TOKEN_GREATER_EQUAL: emitByte(OP_GREATER_EQUAL); cout << "greater equal\n"; break;
        case TOKEN_LESS_EQUAL: emitByte(OP_LESS_EQUAL); cout << "less equal\n"; break;
        case TOKEN_NOT_EQUAL: emitByte(OP_NOT_EQUAL); cout << "not equal\n"; break;

        case TOKEN_NOT: emitByte(OP_NOT); cout << "not\n"; break;
        default: return; // unreachable
    }
}
static uint8_t identifierConstant(token* name)
{
    return addConstant(currentChunk(), OBJ_VAL( copyString(name->text) ));
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
            case TOKEN_FALSE: emitByte(OP_FALSE); break;
            case TOKEN_NIL: emitByte(OP_NIL); break;
            case TOKEN_TRUE: emitByte(OP_TRUE); break;

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
            case TOKEN_STRING:
            {
                emitConstant(OBJ_VAL( copyString(parser.current->text) ));
                break;
            }
            case TOKEN_IDENTIFIER:
            {
                uint8_t arg = identifierConstant(parser.current);
                emitBytes(OP_GET_GLOBAL, arg);
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
static void expressionStatement()
{
    expression();
    emitByte(OP_POP);
}
static void printStatement()
{
    expression();
    emitByte(OP_ECHO);
}
static bool check(TokenType type)
{
    return parser.current->type == type;
}
static bool match(TokenType type)
{
    if (!check(type)) return false;
    advance();
    return true;
}
static void statement()
{
    if (match(TOKEN_ECHO))
    {
        printStatement();
    }
    else
    {
        expressionStatement();
    }
}
static uint8_t parseVariable(string errorMessage)
{
    consume(TOKEN_IDENTIFIER, errorMessage);
    return identifierConstant(parser.previous);
}
static void defineVariable(uint8_t global)
{
    emitBytes(OP_DEFINE_GLOBAL, global);
}
static void varDeclaration()
{
    uint8_t global = parseVariable("expect variable name");

    if(match(TOKEN_EQUAL))
    {
        expression();
    }
    else
    {
        emitByte(OP_NIL);
    }
    defineVariable(global);
}
static void declaration()
{
    if(match(TOKEN_VAR))
    {
        varDeclaration();
    }
    else
    {
        statement();
    }
}
bool compile(string source, chunk* chunk)
{
    tokens.clear();
    generateTokens(source);
    compilingChunk = chunk;
    parser.current = &tokens[0];
    parser.hadError = false;    

    for (token t : tokens) {
        cout << t.line << " | " << "type: " << t.type << "| " << t.text << "\n";
    }

    while (!match(TOKEN_EOF)) {
        declaration();
    }
    parser.current--;
    emitByte(OP_RETURN);
    return !parser.hadError;
}
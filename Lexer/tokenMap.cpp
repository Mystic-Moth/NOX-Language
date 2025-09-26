#include <unordered_map>
#include "lexer.h"
#include "tokenmap.h"

using namespace std;

unordered_map<string, TokenType> operatorMap = {
    {"+", TOKEN_PLUS},
    {"-", TOKEN_MINUS},
    {"/", TOKEN_SLASH},
    {"*", TOKEN_STAR},

    {";", TOKEN_SEMICOLON},

    {".", TOKEN_DOT},
    {",", TOKEN_COMMA},

    {"{", TOKEN_LEFT_BRACE},
    {"}", TOKEN_RIGHT_BRACE},
    {"(", TOKEN_LEFT_PAREN},
    {")", TOKEN_RIGHT_PAREN},

    {"!", TOKEN_NOT},
    {"=", TOKEN_EQUAL},
    {"<", TOKEN_GREATER},
    {">", TOKEN_LESS},

    {"==", TOKEN_EQUAL_EQUAL},
    {"<=", TOKEN_GREATER_EQUAL},
    {">=", TOKEN_LESS_EQUAL},
    {"!=", TOKEN_NOT_EQUAL}
};

unordered_map<string, TokenType> keywordMap = {
    {"v ", TOKEN_VAR},
    {"var", TOKEN_VAR},

    {"for", TOKEN_FOR},
    {"foreach", TOKEN_FOREACH},

    {"if", TOKEN_IF},
    {"else", TOKEN_ELSE},

    {"and", TOKEN_AND},
    {"or", TOKEN_OR},

    {"func", TOKEN_FUNC},
    {"return", TOKEN_RETURN},
    {"nil", TOKEN_NIL},
    
    {"true", TOKEN_TRUE},
    {"false", TOKEN_FALSE},

    {"echo", TOKEN_ECHO},
};

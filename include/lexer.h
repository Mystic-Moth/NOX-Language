#ifndef TOKENGENERATION_H 
#define TOKENGENERATION_H
    #include <string>
    #include <vector>

    void generateTokens(std::string code);

    enum TokenType {
        TOKEN_ERROR, TOKEN_EOF,
        // single character
        TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
        TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
        TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
        TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,

        // two character
        TOKEN_NOT, TOKEN_NOT_EQUAL,
        TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
        TOKEN_GREATER, TOKEN_GREATER_EQUAL,
        TOKEN_LESS, TOKEN_LESS_EQUAL,

        // literals
        TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,

        // keywords
        TOKEN_TRUE, TOKEN_FALSE,
        TOKEN_AND, TOKEN_FOR, TOKEN_OR,
        TOKEN_WHILE, TOKEN_FUNC, TOKEN_IF, TOKEN_ELSE, TOKEN_FOREACH,
        TOKEN_ECHO, TOKEN_RETURN, TOKEN_NIL,
        TOKEN_VAR
    };
    
    struct token {
        TokenType type;
        std::string text;
        int length;
        int line;
    };
    extern std::vector<token> tokens;

#endif
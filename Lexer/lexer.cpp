#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "lexer.h"
#include "tokenmap.h"

using namespace std;

struct scanner
{
    int start;
    int current;
    int line;
};
struct trieNode
{
    TokenType token;
    trieNode* children[94]; // 94 usable ASCII characters from ' ' to '~'

    trieNode()
    {
        token = TOKEN_ERROR;
        for (int i = 0; i < 94; i++) {
            children[i] = nullptr;
        }
    }
};
struct trieTree
{
    trieNode* root;

    trieTree()
    {
        root = new trieNode();
    }
};
trieTree keywordTree;
trieTree operatorTree;
static struct scanner scanner;
vector<token> tokens;
vector<string> lines;
string code;

void trieInsert(string word, unordered_map<string, TokenType> map, trieTree tree)
{
    trieNode *node = tree.root;
    for (char c : word)
    {
        int index = c - ' ';
        if(!node->children[index]) // check if letter branch already exists
        {
            node->children[index] = new trieNode(); // if not make new branch
        }
        node = node->children[index];
    }
    node->token = map[word];
}
trieNode *traverse(trieNode *node, char c)
{
    int index = c - ' ';
    return node->children[index];
}
void createTrieTree(unordered_map<string, TokenType> map, trieTree tree)
{
    for (auto pair : map)
    {
        trieInsert(pair.first, map, tree);
    } 
}
bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
bool isNumber(char c)
{
    return c >= '0' && c <= '9';
}
void generateErrorToken()
{

}
void generateToken(TokenType type)
{
    token token;
    token.length = scanner.current - scanner.start + 1;
    token.text = code.substr(scanner.start, token.length);
    token.type = type;
    token.line = scanner.line;
    tokens.push_back(token);
}
void generateStringToken()
{
    // different from other tokens cuz we don't want the quotes "
    token token;
    token.length = scanner.current - scanner.start - 1; // + 1;
    token.text = code.substr(scanner.start+1, token.length);
    token.type = TOKEN_STRING;
    token.line = scanner.line;
    tokens.push_back(token);
}
char peek()
{
    if (scanner.current == code.length()-1) return '\0';
    return code[scanner.current];
}
char peekNext()
{
    if (scanner.current+1 >= code.length()-1) return '\0';
    return code[scanner.current+1];
}
void tokeniseNumber()
{
    while (isNumber(peek()))
    {
        scanner.current++;
    }
    scanner.current--;
    generateToken(TOKEN_NUMBER);
}
void tokeniseIdentifier()
{
    trieNode *node = keywordTree.root;
    while (traverse(node, peek()))
    {
        node = traverse(node, peek());
        scanner.current++;
    }
    if (node->token != TOKEN_ERROR)
    {
        scanner.current--;
        generateToken(node->token);
    }
    else
    {
        while (isalpha(peekNext()))
        {
            scanner.current++;
        }
        generateToken(TOKEN_IDENTIFIER);
    }
}
void tokeniseString()
{
    scanner.current++; // skip opening "
    while (peek() != '"' && peek() != '\0')
    {
        if (peek() == '\n') scanner.line++;
        scanner.current++;
    }
    if (peek() == '\0')
    {
        generateErrorToken();
        return;
    }
    generateStringToken();
}
void tokeniseOperator()
{
    trieNode *node = operatorTree.root;
    while (traverse(node, peek()))
    {
        node = traverse(node, peek());
        scanner.current++;
    }
    scanner.current--;
    generateToken(node->token);
}
void skipWhitespace()
{
    while (true)
    {
        switch(peek())
        {
            case ' ': scanner.current++; break;
            case '\n': scanner.current++; scanner.line++; break;
            case '/':
                if (peekNext() == '/')
                {
                    while (peek() != '\n')
                    {
                        scanner.current++;
                    }
                    scanner.line++;
                    scanner.current++;
                }

            default: return;

        }
    }
}
void generateTokens(string text)
{
    code = text;
    // create trie trees
    // they get created really fast its neglible
    createTrieTree(operatorMap, operatorTree);
    createTrieTree(keywordMap, keywordTree);
    scanner.start = 0;
    scanner.line = 0;

    for (scanner.current = 0; scanner.current < code.size(); scanner.current++)
    {
        skipWhitespace();
        scanner.start = scanner.current;
        if (peek() == '\0') break; // end of file
        if (isNumber(peek()))
        {
            tokeniseNumber();
            continue;
        }
        // is a word
        if (isAlpha(peek()))
        {
            tokeniseIdentifier();
            continue;
        }
        // is a string
        if (peek() == '"')
        {
            tokeniseString();
            continue;
        }

        // that leaves just operators
        tokeniseOperator();
    }
    generateToken(TOKEN_EOF);
}
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Lexer/lexer.h"
#include "chunk.h"
#include "vm.h"

using namespace std;

int main(int argc, char *argv[]) {
    ifstream file("NOX_Code/Code.nox");
    string source;
    string tmp;
    while(getline(file, tmp)) {
        source += tmp + '\n';
        cout << tmp << '\n';
    }

    generateTokens(source);
    for (token t : tokens) {
        cout << t.line << " | " << "type: " << t.type << "|" << t.text << "|" << "\n";
    }

    initVM();

    
}
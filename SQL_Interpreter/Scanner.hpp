#ifndef __SCANNER__
#define __SCANNER__

#include <string>
#include <map>
#include <iostream>

#include "Lexeme.hpp"

using std::string;
using std::map;

class Scanner {
    enum State { 
        H, ID, NUM, MUL, COMMA, SMCLN, RP, LP, DIV, 
        PLUS, MIN, PERC, EQ, G , L, NE, AP, END 
    };
    
    FILE *infile;
    State cs;
    int c = ' ';
    std::string buf;
    
    static map <string, Lexeme_type> TW;
    
    void clear();
    void add();
    void gc();
    Lexeme_type check_TW(const string);
    State getcs(int) const;
    
public:
    static map<Lexeme_type, string> types;
    
    Lexeme get_lex();
    
    void print_TID() const;
};

#endif /* __SCANNER__ */
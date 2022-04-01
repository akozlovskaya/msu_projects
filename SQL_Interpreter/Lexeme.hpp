#ifndef __LEXEME__
#define __LEXEME__

#include <string>
#include <iostream>

using std::string;

enum Lexeme_type {
        LEX_NAME, LEX_STR, LEX_UNS, LEX_SELECT, LEX_FROM, LEX_INSERT, LEX_INTO,
        LEX_UPDATE, LEX_SET, LEX_DELETE, LEX_DROP, LEX_CREATE, LEX_TABLE,
        LEX_WHERE, LEX_NOT, LEX_OR, LEX_AND, LEX_LIKE, LEX_IN, LEX_ALL,
        LEX_TEXT, LEX_LONG, LEX_MUL, LEX_DIV, LEX_PLUS, LEX_MIN, LEX_PERC,
        LEX_GR, LEX_LESS, LEX_EQ, LEX_NE, LEX_GE, LEX_LE,
        LEX_LP, LEX_RP, LEX_COMMA, LEX_APOSTR, LEX_END
};

class Lexeme {
    Lexeme_type type;
    
    string str = {0};
public:
    Lexeme(Lexeme_type, string);
    
    Lexeme_type get_type() const;
    string get_str() const;
};

#endif /* __LEXEME__ */
#ifndef __PARSER__
#define __PARSER__

#include <string>
#include <stack>
#include <vector>
#include <iostream>

#include "Lexeme.hpp"
#include "Scanner.hpp"
#include "Query.hpp"

using std::string;
using std::stack;
using std::vector;
using std::cout;
using std::endl;


class Parser {
    Lexeme_type cur_t;
    string cur_str;
    Scanner scan;
    Query query;
    
    void S();
    void SELECT();
    void INSERT();
    void UPDATE();
    void DELETE();
    void CREATE();
    void DROP();
    void FIELDS();
    void FV();
    void FD();
    void TYPE();
    void LONG(long &);
    void WHERE();
    void WHERE1();
    void WHERE2();
    void CONST();
    void OP1(Poliz_type&);
    void OP2(Poliz_type&);
    void OP3(Poliz_type&);
    void TERM();
    void MULT();
    void VALUE();
    void EXPR();
    void EXPR1();
    
    void gl();
public:
    Parser() = default;
    ~Parser() = default;

    bool analyze();
    bool isend();
    void print_poliz();
    const Query & get_query() const;
};
#endif /* __PARSER__ */
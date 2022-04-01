#include "Parser.hpp"

//--------------------------------------------------------------//

void Parser::gl()
{
    Lexeme lex = scan.get_lex();
    cur_t = lex.get_type();
    cur_str = lex.get_str();
}
//--------------------------------------------------------------//
    
bool Parser::analyze ()
{
    try {
        gl();
        S();
        return true;
    } catch (...) {
        return false;
    }
}

void Parser::S()
{
    if (cur_t == LEX_SELECT) {
        query.set_type(Select);
        gl();
        SELECT();
    } else if (cur_t == LEX_INSERT) {
        query.set_type(Insert);
        gl();
        INSERT();
    } else if (cur_t == LEX_UPDATE) {
        query.set_type(Upd);
        gl();
        UPDATE();
    } else if (cur_t == LEX_DELETE) {
        query.set_type(Del);
        gl();
        DELETE();
    } else if (cur_t == LEX_CREATE) {
        query.set_type(Create);
        gl();
        CREATE();
    } else if (cur_t == LEX_DROP) {
        query.set_type(Drop);
        gl();
        DROP();
    } else if (cur_t == LEX_NAME) {
        if (cur_str != "q") throw cur_t;
        query.set_type(Q);
    } else throw cur_t;
    if (cur_t != LEX_END) throw cur_t; 
}
////////////////////////////////////////////////////////////////
//SELECT -> FIELDS from name WHERE
void Parser::SELECT()
{
    FIELDS();
    if (cur_t != LEX_FROM) throw cur_t;
    gl();
    if (cur_t != LEX_NAME) throw cur_t;
    query.set_name(cur_str);
    gl();
    WHERE();
}
//INSERT -> into name ( FV { , FV })
void Parser::INSERT()
{
    if (cur_t != LEX_INTO) throw cur_t;
    gl();
    if (cur_t != LEX_NAME) throw cur_t;
    query.set_name(cur_str);
    gl();
    
    if (cur_t != LEX_LP) throw cur_t;
    gl();
    FV();
    while (cur_t == LEX_COMMA) {
        gl();
        FV();
    }
    if (cur_t != LEX_RP) throw cur_t;
    gl();
}
//UPDATE -> name set name = EXPR WHERE
void Parser::UPDATE()
{
    if (cur_t != LEX_NAME) throw cur_t;
    query.set_name(cur_str);
    gl();
    if (cur_t != LEX_SET) throw cur_t;
    gl();
    if (cur_t != LEX_NAME) throw cur_t;
    query.add_data(Poliz_Name, cur_str);
    gl();
    if (cur_t != LEX_EQ) throw cur_t;
    gl();
    EXPR();
    query.cond_to_expr();
    WHERE();
}

//DELETE -> from name WHERE
void Parser::DELETE()
{
    if (cur_t != LEX_FROM) throw cur_t;
    gl();
    if (cur_t != LEX_NAME) throw cur_t;
    query.set_name(cur_str);
    gl();
    WHERE();
}

//CREATE -> table name ( FD { , FD } )
void Parser::CREATE()
{
    if (cur_t != LEX_TABLE) throw cur_t;
    gl();
    if (cur_t != LEX_NAME) throw cur_t;
    query.set_name(cur_str);
    gl();
    if (cur_t != LEX_LP) throw cur_t;
    gl();
    FD();
    while (cur_t == LEX_COMMA) {
        gl();
        FD();
    }
    if (cur_t != LEX_RP) throw cur_t;
    gl();
}

//DROP -> table name
void Parser::DROP()
{
    if (cur_t != LEX_TABLE) throw cur_t;
    gl();
    if (cur_t != LEX_NAME) throw cur_t;
    query.set_name(cur_str);
    gl();
}

//FIELDS -> name { , name } | *
void Parser::FIELDS()
{
    if (cur_t == LEX_NAME) {
        query.add_data(Poliz_Name, cur_str);
        gl();
        while (cur_t == LEX_COMMA) {
            gl();
            if (cur_t != LEX_NAME) throw cur_t;
            query.add_data(Poliz_Name, cur_str);
            gl();
        }
    } else if (cur_t == LEX_MUL) {
        query.add_data(Poliz_All);
        gl();
    } else throw cur_t;
}
//FV -> lex_str | LONG
void Parser::FV()
{
    if (cur_t == LEX_STR) {
        query.add_data(Poliz_Str, cur_str); 
        gl();
    } else if (cur_t == LEX_UNS || cur_t == LEX_MIN) {
        long num;
        LONG(num);
        query.add_data(Poliz_Num, num);
    } else throw cur_t;
}

//FD -> name TYPE
void Parser::FD()
{
    if (cur_t != LEX_NAME) throw cur_t;
    query.add_data(Poliz_Name, cur_str);
    gl();
    TYPE();
}

//TYPE -> text ( lex_uns ) | long
void Parser::TYPE()
{
    if (cur_t == LEX_TEXT) {
        gl();
        if (cur_t != LEX_LP) throw cur_t;
        gl();
        if (cur_t != LEX_UNS) throw cur_t;
        query.add_data(Poliz_Fd_text, std::stol(cur_str));
        gl();
        if (cur_t != LEX_RP) throw cur_t;
        gl();
    } else if (cur_t == LEX_LONG) {
        query.add_data(Poliz_Fd_long, sizeof(long));
        gl();
    } else throw cur_t;
}

//LONG -> [-] LEX_UNS
void Parser::LONG(long &num)
{
    int sign = 1;
    if (cur_t == LEX_MIN) {
        sign = -1;
        gl();
    }
    if (cur_t != LEX_UNS) throw cur_t;
    num = sign*std::stol(cur_str);
    gl();
}

//WHERE —> lex_where WHERE1
void Parser::WHERE()
{
    if (cur_t != LEX_WHERE) throw cur_t;
    gl();
    WHERE1();
}


//WHERE1 —> EXPR WHERE2 | lex_all
void Parser::WHERE1()
{
    if (cur_t == LEX_ALL) {
        query.set_cond(All); 
        gl();
    } else {
        EXPR();
        WHERE2();
    }
}

//WHERE2 —> [ lex_not ] lex_like lex_str | [ lex_not ] lex_in ( CONST ) | eps
void Parser::WHERE2()
{
    if (cur_t == LEX_NOT) {
        query.set_neg(true);
        gl();
    }
    
    if (cur_t == LEX_LIKE) {
        gl();
        if (cur_t != LEX_STR) throw cur_t;
        query.set_cond(Like);
        query.add_const(Poliz_Str, cur_str); 
        gl();
    } else if (cur_t == LEX_IN) {
        gl();
        if (cur_t != LEX_LP) throw cur_t;
        gl();
        query.set_cond(In);
        CONST();
        if (cur_t != LEX_RP) throw cur_t;
        gl();
    } else {
        query.set_cond(Expr);
    }
}

//CONST —> lex_str { , lex_str } | LONG { , LONG }
void Parser::CONST()
{
    if (cur_t == LEX_STR) {
        query.add_const(Poliz_Str, cur_str);
        gl();
        while (cur_t == LEX_COMMA) {
            gl();
            if (cur_t != LEX_STR) throw cur_t;
            query.add_const(Poliz_Str, cur_str);
            gl();
        }
    } else if (cur_t == LEX_MIN || cur_t == LEX_UNS) {
        long num;
        LONG(num);
        query.add_const(Poliz_Num, num);
        while (cur_t == LEX_COMMA) {
            gl();
            LONG(num);
            query.add_const(Poliz_Num, num);
        }
    } else throw cur_t;
}

//EXPR —> TERM { OP2 TERM }
void Parser::EXPR()
{
    TERM();
    while (cur_t == LEX_PLUS || cur_t == LEX_MIN || cur_t == LEX_OR) {
        Poliz_type op;
        OP2(op);
        TERM();
        query.add_cond(op);
    }
}

//OP2 —> + | - | or
void Parser::OP2(Poliz_type &op)
{
    if (cur_t == LEX_PLUS) {
        op = Poliz_plus;
        gl();
    } else if (cur_t == LEX_MIN) {
        op = Poliz_min;
        gl();
    } else if (cur_t == LEX_OR) {
        op = Poliz_or;
        gl();
    } else throw cur_t;
}

//TERM —> MULT { OP1 MULT }
void Parser::TERM()
{
    MULT();
    while (cur_t == LEX_MUL || cur_t == LEX_DIV || cur_t == LEX_PERC || cur_t == LEX_AND) {
        Poliz_type op;
        OP1(op);
        MULT();
        query.add_cond(op);
    }
}

//OP1 —> * | / | % | and
void Parser::OP1(Poliz_type &op)
{
    if (cur_t == LEX_MUL) {
        op = Poliz_mul;
        gl();
    } else if (cur_t == LEX_DIV) {
        op = Poliz_div;
        gl();
    } else if (cur_t == LEX_PERC) {
        op = Poliz_perc;
        gl();
    } else if (cur_t == LEX_AND) {
        op = Poliz_and;
        gl();
    } else throw cur_t;
}

//MULT —> VALUE | ( EXPR1 ) | lex_not MULT
void Parser::MULT()
{
    if (cur_t == LEX_NAME || cur_t == LEX_MIN || cur_t == LEX_UNS || cur_t == LEX_STR) {
        VALUE();
    } else if (cur_t == LEX_LP) {
        gl();
        EXPR1();
        if (cur_t != LEX_RP) throw cur_t;
        gl();
    } else if (cur_t == LEX_NOT) {
        gl();
        MULT();
        query.add_cond(Poliz_not); 
    } else throw cur_t;
}

//VALUE —> name | LONG | lex_str
void Parser::VALUE()
{
    if (cur_t == LEX_MIN || cur_t == LEX_UNS) {
        long num;
        LONG(num);
        query.add_cond(Poliz_Num, num);
    } else if (cur_t == LEX_NAME) {
        query.add_cond(Poliz_Name, cur_str);
        gl();
    } else if (cur_t == LEX_STR) {
        query.add_cond(Poliz_Str, cur_str);
        gl();
    } else throw cur_t;
}

//OP3 —> = | > | < | >= | <= | !=
void Parser::OP3(Poliz_type &op)
{
    if (cur_t == LEX_EQ) {
        op = Poliz_eq;
        gl();
    } else if (cur_t == LEX_NE) {
        op = Poliz_ne;
        gl();
    } else if (cur_t == LEX_GR) {
        op = Poliz_gr;
        gl();
    } else if (cur_t == LEX_LESS) {
        op = Poliz_less;
        gl();
    } else if (cur_t == LEX_GE) {
        op = Poliz_ge;
        gl();
    } else if (cur_t == LEX_LE) {
        op = Poliz_le;
        gl();
    } else throw cur_t;
}

//EXPR2 —> EXPR { OP3 EXPR }
void Parser::EXPR1()
{
    EXPR();
    while (cur_t == LEX_EQ || cur_t == LEX_NE || cur_t == LEX_GR ||
           cur_t == LEX_LESS || cur_t == LEX_GE || cur_t == LEX_LE) {
        Poliz_type op;
        OP3(op);
        EXPR();
        query.add_cond(op);
    }
}

bool Parser::isend()
{
    if (query.get_type() == Q) return true;
    return false;
}

void Parser::print_poliz()
{
    query.print();
}

const Query &
Parser::get_query() const {
    return query;
}
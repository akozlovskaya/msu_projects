#include "Scanner.hpp"

map<string, Lexeme_type>
Scanner::TW = { 
    { "SELECT", LEX_SELECT }, 
    { "FROM", LEX_FROM }, 
    { "INSERT", LEX_INSERT }, 
    { "INTO", LEX_INTO }, 
    { "UPDATE", LEX_UPDATE }, 
    { "SET", LEX_SET }, 
    { "DELETE", LEX_DELETE }, 
    { "FROM", LEX_FROM }, 
    { "CREATE", LEX_CREATE }, 
    { "TABLE", LEX_TABLE }, 
    { "WHERE", LEX_WHERE }, 
    { "NOT", LEX_NOT }, 
    { "OR", LEX_OR }, 
    { "AND", LEX_AND }, 
    { "LIKE", LEX_LIKE }, 
    { "IN", LEX_IN }, 
    { "ALL", LEX_ALL }, 
    { "TEXT", LEX_TEXT }, 
    { "LONG", LEX_LONG }, 
    { "DROP", LEX_DROP }
};

void Scanner::gc() { c = fgetc(stdin); }

void Scanner::clear() { buf.clear(); }

void Scanner::add() { buf += c; }

Scanner::State
Scanner::getcs(int sym) const
{
    if (sym == EOF || sym == ';') return END;
    else if (isspace(sym)) return H;
    else if (sym == '(') return LP;
    else if (sym == ')') return RP;
    else if (sym == '*') return MUL;
    else if (sym == ',') return COMMA;
    else if (sym == '/') return DIV;
    else if (sym == '+') return PLUS;
    else if (sym == '-') return MIN;
    else if (sym == '%') return PERC;
    else if (sym == '=') return EQ;
    else if (sym == '>') return G;
    else if (sym == '<') return L;
    else if (sym == '!') return NE;
    else if (sym == '\'') return AP;
    throw sym;
}

Lexeme_type
Scanner::check_TW(const string name)
{
    map<string, Lexeme_type>::iterator it;
    it = TW.find(name);
    if (it == TW.end()) {
        return LEX_NAME;
    }
    return TW[name];
}

Lexeme
Scanner::get_lex ()
{
    cs = H;
    do {
        switch (cs) {
        case H:
            clear();
            add();
            if (isalpha(c) || c == '_') {
                cs = ID;
            } else if (isdigit(c)) {
                cs = NUM;
            } else {
                cs = getcs(c);
            }
            gc();
            break;
        case ID:
            if (isalpha(c) || isdigit(c) || c == '_') {
                add();
                gc();
            } else {
                cs = getcs(c);
                return Lexeme(check_TW(buf), buf);
            }
            break;
        case NUM:
            if (isdigit(c)) {
                add();
                gc();
            } else {
                cs = getcs(c);
                return Lexeme(LEX_UNS, buf);
            }
            break;
        case G:
            if (c == '=') {
                add();
                gc();
                return Lexeme(LEX_GE, buf);
            } else {
                return Lexeme(LEX_GR, buf);
            }
            break;
        case L:
            if (c == '=') {
                add();
                gc();
                return Lexeme(LEX_LE, buf);
            } else {
                return Lexeme(LEX_LESS, buf);
            }
            break;
        case NE:
            if (c == '=') {
                add();
                gc();
                return Lexeme(LEX_NE, buf);
            } else throw c;
            break;
        case AP:
            if (c == '\'') {
                gc();
                buf.erase(0, 1);
                return Lexeme(LEX_STR, buf);
            }
            add();
            gc();
            break;
        case LP:
            return Lexeme(LEX_LP, buf);
        case RP:
            return Lexeme(LEX_RP, buf);
        case COMMA:
            return Lexeme(LEX_COMMA, buf);
        case PERC:
            return Lexeme(LEX_PERC, buf);
        case MUL:
            return Lexeme(LEX_MUL, buf);
        case DIV:
            return Lexeme(LEX_DIV, buf);
        case PLUS:
            return Lexeme(LEX_PLUS, buf);
        case MIN:
            return Lexeme(LEX_MIN, buf);
        case EQ:
            return Lexeme(LEX_EQ, buf);
        case END:
            return Lexeme(LEX_END, "END");
        default:
            break;
        }
    } while (true);
}
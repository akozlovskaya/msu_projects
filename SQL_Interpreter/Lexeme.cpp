#include "Lexeme.hpp"

Lexeme::Lexeme(Lexeme_type type_, string str_ = "") : type(type_), str(str_) {}

Lexeme_type Lexeme::get_type() const 
{
    return type;
}

string Lexeme::get_str() const 
{
    return str;
}
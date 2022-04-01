#include "Poliz_elem.hpp"

Poliz_elem::Poliz_elem() {}

Poliz_elem::Poliz_elem(Poliz_type type_) : type(type_) {}

Poliz_elem::Poliz_elem(Poliz_type type_, string str_) : type(type_)
{
    str = str_;
}

Poliz_elem::Poliz_elem(Poliz_type type_, int num_) : type(type_), num(num_) {}

Poliz_elem::Poliz_elem(Poliz_type type_, int num_, string str_) : type(type_), num(num_)
{
    str = str_;
}

void Poliz_elem::print() const
{
    std::cout << type << ", " << str << ", " << num << std::endl;
}

Poliz_type Poliz_elem::get_poliz_type() const
{
    return type;
}

const string & Poliz_elem::get_poliz_str() const
{
    return str;
}

long Poliz_elem::get_poliz_num() const
{
    return num;
}
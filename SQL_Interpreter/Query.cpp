#include "Query.hpp"

void Query::cond_to_expr()
{
    expr_poliz = cond_poliz;
    cond_poliz.clear();
}

Clause_type
Query::get_type() const
{
    return cl_type;
}
const string &
Query::get_name() const
{
    return table_name;
}
Condition_type
Query::get_cond() const
{
    return cond_type;
}
bool
Query::get_neg() const
{
    return neg;
}

void Query::set_type(Clause_type t)
{
    cl_type = t;
}

void Query::set_name(const string &name)
{
    table_name = name;
}
void Query::set_cond(Condition_type cond)
{
    cond_type = cond;
}

void Query::set_neg(bool b)
{
    neg = b;
}

void Query::add_data(Poliz_type type_)
{
    data.push_back(Poliz_elem(type_));
}

void Query::add_data(Poliz_type type_, int num_)
{
    data.push_back(Poliz_elem(type_, num_));
}

void Query::add_data(Poliz_type type_, string str_)
{
    data.push_back(Poliz_elem(type_, str_));
}

void Query::add_data(Poliz_type type_, int num_, string str_)
{
    data.push_back(Poliz_elem(type_, num_, str_));
}

void Query::add_cond(Poliz_type type_)
{
    cond_poliz.push_back(Poliz_elem(type_));
}

void Query::add_cond(Poliz_type type_, int num_)
{
    cond_poliz.push_back(Poliz_elem(type_, num_));
}

void Query::add_cond(Poliz_type type_, string str_)
{
    cond_poliz.push_back(Poliz_elem(type_, str_));
}

void Query::add_cond(Poliz_type type_, int num_, string str_)
{
    cond_poliz.push_back(Poliz_elem(type_, num_, str_));
}

void Query::add_const(Poliz_type type_)
{
    constants.push_back(Poliz_elem(type_));
}

void Query::add_const(Poliz_type type_, int num_)
{
    constants.push_back(Poliz_elem(type_, num_));
}

void Query::add_const(Poliz_type type_, string str_)
{
    constants.push_back(Poliz_elem(type_, str_));
}

void Query::add_const(Poliz_type type_, int num_, string str_)
{
    constants.push_back(Poliz_elem(type_, num_, str_));
}

const vector <Poliz_elem> & 
Query::get_data() const
{
    return data;
}
const vector <Poliz_elem> &
Query::get_expr_poliz() const
{
    return expr_poliz;
}
const vector <Poliz_elem> &
Query::get_cond_poliz() const
{
    return cond_poliz;
}
const vector <Poliz_elem> &
Query::get_const() const
{
    return constants;
}

void Query::print() const
{
    std::cout << "Table name is " << table_name << std::endl;
    std::cout << "      data" << std::endl;
    for (size_t i = 0; i < data.size(); ++i) {
        data[i].print();
    }
    std::cout << "      expr_poliz" << std::endl;
    for (size_t i = 0; i < expr_poliz.size(); ++i) {
        expr_poliz[i].print();
    }
    std::cout << "      cond_poliz" << std::endl;
    for (size_t i = 0; i < cond_poliz.size(); ++i) {
        cond_poliz[i].print();
    }
    std::cout << "      constants" << std::endl;
    for (size_t i = 0; i < constants.size(); ++i) {
        constants[i].print();
    }
}
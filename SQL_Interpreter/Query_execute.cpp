#include "Query.hpp"

list<string>
Query::execute()
{

    switch (cl_type) {
        case Select:
            return select();
        case Insert:
            return insert();
        case Upd:
            return update();
        case Del:
            return dlt();
        case Create:
            return create();
        case Drop:
            return drop();
        default:
            list<string> ret;
            return ret;
    }
}

using std::cout;
using std::endl;

list<string>
Query::drop()
{
    list<string> ret;
    try {
        THandle th;
        if (isopened(table_name, th)) {
            close(th);
        }
        delete_table(table_name);
        ret.push_back(success_msg);
    } catch (...) {
        ret.push_back(Query::system_error);
    }
    return ret;
}

list<string>
Query::create()
{
    list<string> ret;
    try {
        unsigned fields_num = data.size() / 2;
        TableStruct table;
        
        table.numOfFields = fields_num;
        table.fieldsDef = new FieldDef [fields_num];
        for (unsigned i = 0; i < fields_num; ++i) {
            
            strcpy(table.fieldsDef[i].name, data[2*i].get_poliz_str().c_str());
            Poliz_elem field = data[2*i + 1];
            
            if (field.get_poliz_type() == Poliz_Fd_text) {
                table.fieldsDef[i].type = Text;
            } else if (field.get_poliz_type() == Poliz_Fd_long) {
                table.fieldsDef[i].type = Long;
            }
            
            table.fieldsDef[i].len = field.get_poliz_num();
        }
        create_table(table_name, &table);
        delete [] table.fieldsDef;
        
        ret.push_back(success_msg);
    } catch (...) {
        ret.push_back(Query::system_error);
    }
    return ret;
}

list<string>
Query::insert()
{
    list<string> ret;
    try {
        THandle th;
        if (!isopened(table_name, th)) th = open(table_name);
        unsigned fd_num = data.size();
        if (fd_num != get_field_num(th)) {
            throw Query::format_error;
        }
        create_rec(th);
        for (unsigned i = 0; i < fd_num; ++i) {
            Poliz_elem field = data[i];
            
            if (field.get_poliz_type() == Poliz_Str
                    && get_field_type(th, i) == Long)
            {
                throw Query::format_error;
            }
            if (field.get_poliz_type() == Poliz_Num
                    && get_field_type(th, i) == Text)
            {
                throw Query::format_error;
            }
            
            if (field.get_poliz_type() == Poliz_Str) {
                put_text_new(th, get_field_name(th, i), field.get_poliz_str());
            } else if (field.get_poliz_type() == Poliz_Num) {
                put_long_new(th, get_field_name(th, i), field.get_poliz_num());
            }
        }
        insert_end(th);
        close(th);
        ret.push_back(success_msg);
    } catch (string err) {
        ret.push_back(err);
    } catch (...) {
        ret.push_back(Query::syntax_error);
    }
    return ret;
}

bool
islong(THandle th, const Poliz_elem &x, long &a)
{
    a = 0;
    if (x.get_poliz_type() == Poliz_Name) {
        if (get_field_type(th, x.get_poliz_str()) != Long) return false;
        a = get_long(th, x.get_poliz_str());
    } else {
        if (x.get_poliz_type() != Poliz_Num) return false;
        a = x.get_poliz_num();
    }
    return true;
}

bool
istext(THandle th, const Poliz_elem &x, string &a)
{
    if (x.get_poliz_type() == Poliz_Name) {
        if (get_field_type(th, x.get_poliz_str()) != Text) return false;
        a = get_text(th, x.get_poliz_str());
    } else {
        if (x.get_poliz_type() != Poliz_Str) return false;
        a = x.get_poliz_str();
    }
    return true;
}

template <typename T> bool
Query::calc_logical(Poliz_type op, const T &x, const T &y)
{
    bool res;
    if (op == Poliz_eq) res = x == y;
    else if (op == Poliz_ne) res = x != y;
    else if (op == Poliz_less) res = x < y;
    else if (op == Poliz_gr) res = x > y;
    else if (op == Poliz_le) res = x <= y;
    else res = x >= y;
    return res;
}

void
Query::calc_op(THandle th, Poliz_type op)
{
    if (op < Poliz_eq || op > Poliz_not) throw Query::syntax_error;
    
    Poliz_elem second = poliz_stack.top();
    poliz_stack.pop();
    
    Poliz_type result_type;
    long result_num = 0;
    
    if (op == Poliz_not) {
        
        if (second.get_poliz_type() != Poliz_Bool) throw Query::syntax_error;
        
        result_type = Poliz_Bool;
        result_num = !second.get_poliz_num();
    } else {
        
        Poliz_elem first = poliz_stack.top();
        poliz_stack.pop();
        
        if (op >= Poliz_eq && op <= Poliz_le) {
            result_type = Poliz_Bool;
            
            long a, b;
            string str1, str2;
            
            if (islong(th, first, a) && islong(th, second, b)) {
                result_num = calc_logical<long>(op, a, b);
            } else if (istext(th, first, str1) && istext(th, second, str2)) {
                result_num = calc_logical<string>(op, str1, str2);
            } else {
                throw Query::syntax_error;
            }
            
        } else if (op == Poliz_and || op == Poliz_or) {
            
            if (first.get_poliz_type() != Poliz_Bool
                    || second.get_poliz_type() != Poliz_Bool)
            {
                throw Query::syntax_error;
            }
            
            result_type = Poliz_Bool;
            if (op == Poliz_and) {
                result_num = first.get_poliz_num() && second.get_poliz_num();
            } else {
                result_num = first.get_poliz_num() || second.get_poliz_num();
            }
        } else {
            result_type = Poliz_Num;
            long a, b;
            if (islong(th, first, a) && islong(th, second, b)) {
                if (op == Poliz_plus) {
                    result_num = a + b;
                } else if (op == Poliz_min) {
                    result_num = a - b;
                } else if (op == Poliz_div) {
                    result_num = a / b;
                } else if (op == Poliz_mul) {
                    result_num = a * b;
                } else if (op == Poliz_perc) {
                    result_num = a % b;
                }
            } else throw Query::syntax_error;
        }
    }
    poliz_stack.push(Poliz_elem(result_type, result_num));
}

bool
Query::calc_like(THandle th, Poliz_elem x)
{    
    if (!(x.get_poliz_type() == Poliz_Name
            && get_field_type(th, x.get_poliz_str()) == Text))
    {
        throw Query::syntax_error;
    }
    
    string str = get_text(th, x.get_poliz_str());
    string rules = constants[0].get_poliz_str();
    
    if (!rules.size()) throw Query::syntax_error;
    
    string rule;
    
    if (rules[0] != '%') {
        rule = get_rule(rules);
        size_t len = rule_len(rule);
        string substr;
        substr.append(str, 0, len);
        if (!str_is_str(substr, rule)) {
            return false;
        }
        str.erase(0, len);
    }
    rule = get_rule(rules);
    while (rule.size()) {
        if (!str_in_str(str, rule)) {
            return false;
        }
        rule = get_rule(rules);
    }
    return true;
}

bool
Query::calc_in(THandle th, Poliz_elem x)
{
    unsigned len = constants.size();
    long a;
    string str;
    if (islong(th, x, a)) {
        if (constants[0].get_poliz_type() != Poliz_Num) {
            throw Query::syntax_error;
        }
        for (unsigned i = 0; i < len; ++i) {
            if (a == constants[i].get_poliz_num()) {
                return true;
            }
        }
        return false;
    } else if (istext(th, x, str)) {
        if (constants[0].get_poliz_type() != Poliz_Str) {
            throw Query::syntax_error;
        }
        for (unsigned i = 0; i < len; ++i) {
            if (str == constants[i].get_poliz_str()) {
                return true;
            }
        }
        return false;
    }
    return false;
}

string
Query::select_row(THandle th, const vector <bool> &fields)
{
    string ret;
    string field;
    unsigned num = get_field_num(th);
    
    for (unsigned j = 0; j < num; ++j) {
        if (!fields[j]) continue;
        
        FieldType type = get_field_type(th, j);
        
        if (type == Long) {
            field = std::to_string(get_long(th, j));
        } else if (type == Text) {
            field = get_text(th, j);
        } else throw Query::format_error;
        
        field.resize(get_field_len(th, j), ' ');
        ret.append(field);
    }
    ret.push_back('\n');
    return ret;
}

Poliz_elem
Query::calc_poliz(THandle th, const vector <Poliz_elem> &poliz)
{
    for (size_t i = 0; i < poliz.size(); ++i) {
        Poliz_elem cur = poliz[i];
        if (cur.get_poliz_type() == Poliz_Num ||
                cur.get_poliz_type() == Poliz_Name ||
                cur.get_poliz_type() == Poliz_Str)
        {
            poliz_stack.push(cur);
        } else {
            calc_op(th, cur.get_poliz_type());
        }
    }
    Poliz_elem ret = poliz_stack.top();
    poliz_stack.pop();
    return ret;
}
bool
Query::check_condition(THandle th)
{
    if (cond_type == All) return true;
    Poliz_elem result = calc_poliz(th, cond_poliz);
    if (cond_type == Expr) {
        if (result.get_poliz_type() != Poliz_Bool) {
            throw Query::syntax_error;
        }
        return result.get_poliz_num();
    }
    bool res = true;
    if (cond_type == Like) {
        res = calc_like(th, result);
    } else if (cond_type == In) {
        res = calc_in(th, result);
    }
    return neg != res;
}

list<string>
Query::select()
{
    list<string> ret;
    try {
        THandle th;
        if (!isopened(table_name, th)) th = open(table_name);
        
        vector <bool> fields(get_field_num(th), false);
        
        if (data[0].get_poliz_type() == Poliz_All) {
            for (size_t i = 0; i < get_field_num(th); ++i) {
                fields[i] = true;
            }
        } else if (data[0].get_poliz_type() == Poliz_Name) {
            size_t fd_num = data.size();
            for (size_t i = 0; i < fd_num; ++i) {
                size_t id = 0;
                if (isfield(th, data[i].get_poliz_str(), id)) {
                    fields[id] = true;
                } else {
                    throw Query::syntax_error;
                }
            }
        }  else {
            throw Query::syntax_error;
        }
        
        move_first(th);
        while (!end(th)) {
            if (check_condition(th)) {
              ret.push_back(select_row(th, fields));
            }
            move_next(th);
        }
        close(th);
    } catch (string err) {
        ret.clear();
        ret.push_back(err);
    } catch (...) {
        ret.clear();
        ret.push_back(Query::syntax_error);
    }
    return ret;
}

void
Query::update_row(THandle th, const string &field_name)
{
    Poliz_elem x = calc_poliz(th, expr_poliz);
    if (get_field_type(th, field_name) == Text) {
        
        string new_text;
        if (!istext(th, x, new_text)) {
            throw Query::format_error;
        }
        
        edit(th);
        put_text(th, field_name, new_text);
        finish_edit(th);
    } else if (get_field_type(th, field_name) == Long) {
        
        long new_long;
        if (!islong(th, x, new_long)) {
            throw Query::format_error;
        }
        
        edit(th);
        put_long(th, field_name, new_long);
        finish_edit(th);
    }
}

list<string>
Query::update()
{
    list<string> ret;
    try {
        THandle th;
        if (!isopened(table_name, th)) th = open(table_name);
        string field_name = data[0].get_poliz_str();
        if (!isfield(th, field_name)) {
            throw Query::syntax_error;
        }
        while (!end(th)) {
            if (check_condition(th)) {
                update_row(th, field_name);
            }
            move_next(th);
        }
        close(th);
        ret.push_back(success_msg);
    } catch (string err) {
        ret.push_back(err);
    } catch (...) {
        ret.push_back(Query::system_error);
    }
    return ret;
}

list<string>
Query::dlt()
{
    list<string> ret;
    try {
        THandle th;
        if (!isopened(table_name, th)) th = open(table_name);
        
        while (!end(th)) {
            if (check_condition(th)) {
                delete_rec(th);
            }
            move_next(th);
        }
        close(th);
        ret.push_back(success_msg);
    } catch (string err) {
        ret.push_back(err);
    } catch (...) {
        ret.push_back(Query::system_error);
    }
    return ret;
}
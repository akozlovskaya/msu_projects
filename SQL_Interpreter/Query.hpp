#ifndef __QUERY__
#define __QUERY__

#include <string>
#include <vector>
#include <list>
#include <stack>
#include <iostream>

#include "RDBMSTable.hpp"
#include "Poliz_elem.hpp"
#include "Query_calc_like.hpp"

using std::string;
using std::vector;
using std::list;
using std::stack;

enum Clause_type {  Select, Insert, Upd, Del, Create, Drop, Q };
                  
enum Condition_type { Like, In, Expr, All };

class Server;

class Query {
    
    Clause_type cl_type;
    string table_name;
    Condition_type cond_type = All;
    bool neg = false;
    
    vector <Poliz_elem> data;
    vector <Poliz_elem> expr_poliz;
    vector <Poliz_elem> cond_poliz;
    vector <Poliz_elem> constants;
    stack <Poliz_elem> poliz_stack;
    
    list<string> execute();

    list<string> drop();
    list<string> create();
    list<string> insert();
    list<string> select();
    list<string> update();
    list<string> dlt();

    void calc_expr(THandle, unsigned);
    void calc_op(THandle, Poliz_type);
    bool calc_like(THandle, Poliz_elem);
    bool calc_in(THandle, Poliz_elem);
    
    template <typename T> bool calc_logical(Poliz_type, const T &, const T &);
    Poliz_elem calc_poliz(THandle, const vector <Poliz_elem> &); 
    string select_row(THandle, const vector <bool> &);
    void update_row(THandle, const string &);
    bool check_condition(THandle);

public:
    void cond_to_expr();
    
    void set_type(Clause_type);
    void set_name(const string &);
    void set_cond(Condition_type);
    void set_neg(bool);

    Clause_type get_type() const;
    const string & get_name() const;
    Condition_type get_cond() const;
    bool get_neg() const;
    
    void add_data(Poliz_type);
    void add_data(Poliz_type, int);
    void add_data(Poliz_type, string);
    void add_data(Poliz_type, int, string);
    void add_cond(Poliz_type);
    void add_cond(Poliz_type, int);
    void add_cond(Poliz_type, string);
    void add_cond(Poliz_type, int, string);
    void add_const(Poliz_type);
    void add_const(Poliz_type, int);
    void add_const(Poliz_type, string);
    void add_const(Poliz_type, int, string);
    
    static string success_msg;
    static string syntax_error;
    static string format_error;
    static string system_error;

    const vector <Poliz_elem> & get_data() const;
    const vector <Poliz_elem> & get_expr_poliz() const;
    const vector <Poliz_elem> & get_cond_poliz() const;
    const vector <Poliz_elem> & get_const() const;

    void print() const;

    friend Server;
};


#endif /* __QUERY__ */
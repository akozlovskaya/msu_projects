#ifndef __POLIZ_ELEM__
#define __POLIZ_ELEM__

#include <string>
#include <iostream>
#include <cstring>

using std::string;

enum Poliz_type { 
    Poliz_Str, Poliz_Num, Poliz_Bool, Poliz_Fd_text, Poliz_Fd_long, Poliz_Name, Poliz_All,
    Poliz_eq, Poliz_ne, Poliz_gr, Poliz_less, Poliz_ge, Poliz_le,
    Poliz_and, Poliz_or, Poliz_plus, Poliz_min, Poliz_perc, Poliz_mul, Poliz_div, Poliz_not
};

class Poliz_elem {
    
    Poliz_type type;
    string str = " ";
    long num = 0;
public:    
    Poliz_elem();
    Poliz_elem(Poliz_type);
    Poliz_elem(Poliz_type, string);
    Poliz_elem(Poliz_type, int);
    Poliz_elem(Poliz_type, int, string);
    
    Poliz_type get_poliz_type() const;
    const string & get_poliz_str() const;
    long get_poliz_num() const;
    void print() const;
};

#endif /* __POLIZ_ELEM__ */
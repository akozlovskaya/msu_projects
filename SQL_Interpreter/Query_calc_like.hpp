#ifndef __CALC_LIKE__
#define __CALC_LIKE__

#include <string>

using std::string;

enum Like_state { SYM, ONE, LIST, END };

bool sym_in_str(char sym, const string &str);
bool str_is_str(const string &str, const string &src);
size_t rule_len(const string &rule);
bool str_in_str(string &str, const string &rule);
string get_rule(string &rulestr);

#endif /* __CALC_LIKE__ */
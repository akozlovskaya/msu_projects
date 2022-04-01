#include "Query_calc_like.hpp"

bool sym_in_str(char sym, const string &str)
{
    if (str.size() == 3 && str[1] == '-') {
        if (sym >= str[0] && sym <= str[2]) return true;
    } else {
        for (auto c:str) {
            if (sym == c) return true;
        }
    }
    return false;
}

bool str_is_str(const string &str, const string &src)
{
    auto it = src.cbegin();
    auto c = str.cbegin();
    
    Like_state rule;
    if (it == src.cend()) {
        rule = END;
    } else if (*it == '_') {
        rule = ONE;
    } else if (*it == '[') {
        rule = LIST;
    } else rule = SYM;
    
    while (c != str.cend()) {
        switch (rule) {
        case SYM:
        {
            if (*c != *it) return false;
            ++it;
            ++c;
            if (it == src.cend()) {
                rule = END;
            } else if (*it == '_') {
                rule = ONE;
            } else if (*it == '[') {
                rule = LIST;
            }
            break;
        }
        case ONE:
        {
            ++c;
            ++it;
            if (it == src.cend()) {
                rule = END;
            } else if (*it == '_') {
                rule = ONE;
            } else if (*it == '[') {
                rule = LIST;
            } else rule = SYM;
            break;
        }
        case LIST:
        {
            string const_list;
            ++it;
            if (it == src.cend()) throw src;
            bool in_str = true;
            if (*it == '^') {
                in_str = false;
            } else {
                const_list = *it;
            }
            while (*it != ']') {
                ++it;
                if (it == src.cend()) throw src;
                if (*it != ']') const_list += *it;
            }
            if (in_str != sym_in_str(*c, const_list)) return false;
            ++it;
            if (it == src.cend()) {
                rule = END;
            } else if (*it == '_') {
                rule = ONE;
            } else if (*it == '[') {
                rule = LIST;
            } else rule = SYM;
            ++c;
            break;
        }
        case END:
            return false;
        default:
            break;
        }
    }
    return true;
}
size_t rule_len(const string &rule)
{
    unsigned len = 0;
    auto it = rule.cbegin();
    while (it != rule.cend()) {
        ++len;
        if (*it == '[') {
            while (*it != ']' && it != rule.cend()) ++it;
            if (it == rule.cend()) throw "WHERE LIKE: syntax error";
        }
        ++it;
    }
    return len;
}
bool str_in_str(string &str, const string &rule)
{
    size_t len = rule_len(rule);
    if (len > str.size()) return false;
    for (unsigned i = 0; i < str.size() - len + 1; ++i) {
        string substr;
        substr.append(str, i, len);
        bool res = str_is_str(substr, rule);
        if (res) {
            str.erase(0, i+len);
            return true;
        }
    }
    return false;
}

string get_rule(string &rulestr)
{
    auto it = rulestr.cbegin();
    string new_rule = "";
    size_t rule_idx = 0;
    while (*it == '%' && it != rulestr.cend()) {
        ++it;
        ++rule_idx;
    }
    if (it == rulestr.cend()) return new_rule;
    rulestr.erase(0, rule_idx);
    size_t rule_len;
    if (sym_in_str('%', rulestr)) {
        rule_len = rulestr.find("%");
    } else {
        rule_len = rulestr.size();
    }
    new_rule.append(rulestr, 0, rule_len);
    rulestr.erase(0, rule_len);
    return new_rule;
}
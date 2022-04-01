#include "RDBMSTable.hpp"

map<string, THandle> TOT = {};

bool
isopened(const string &name, THandle &th)
{
    map<string, THandle>::iterator it = TOT.find(name);
    if (it != TOT.end()) {
        th = it->second;
        return true;
    }
    else return false; 
}

void
create_table(const string &name, struct TableStruct *ts)
{
    Errors res = createTable(name.c_str(), ts);
    if (res != OK) throw res;
}

void
delete_table(const string &name)
{
    Errors res = deleteTable(name.c_str());
    if (res != OK) throw res;
}

THandle
open(const string &name)
{
    THandle th;
    Errors res = openTable(name.c_str(), &th);
    if (res != OK) throw res;
    TOT[name] = th;
    return th;
}

void
close(THandle th)
{
    Errors res = closeTable(th);
    if (res != OK) throw res;
    map <string, THandle> :: iterator it = TOT.begin();
    while (it != TOT.end()) {
        if(it->second == th) break;
        ++it;
    }
    if (it != TOT.end()) {
      TOT.erase(it);  
    }
}

void
move_first(THandle th)
{
    Errors res = moveFirst(th);
    if (res != OK) throw res;
}

void
move_last(THandle th)
{
    Errors res = moveLast(th);
    if (res != OK) throw res;
}

void
move_next(THandle th)
{
    Errors res = moveNext(th);
    if (res != OK) throw res;
}

void
move_prev(THandle th)
{
    Errors res = movePrevios(th);
    if (res != OK) throw res;
}

bool
begin(THandle th)
{
    return beforeFirst(th);
}

bool
end(THandle th)
{
    return afterLast(th);
}

string
get_text(THandle th, const string &fn)
{
    char *buf;
    Errors res = getText(th, fn.c_str(), &buf);
    if (res != OK) throw res;
    return string(buf);
}

long
get_long(THandle th, const string &fn)
{
    long ret;
    Errors res = getLong(th, fn.c_str(), &ret);
    if (res != OK) throw res;
    return ret;
}

string
get_text(THandle th, unsigned idx)
{
    char *name;
    char *buf;
    Errors res = getFieldName(th, idx, &name);
    if (res != OK) throw res;
    res = getText(th, name, &buf);
    if (res != OK) throw res;
    return string(buf);
}

long
get_long(THandle th, unsigned idx)
{
    long ret;
    char *buf;
    Errors res = getFieldName(th, idx, &buf);
    if (res != OK) throw res;
    res = getLong(th, buf, &ret);
    if (res != OK) throw res;
    return ret;
}

void
edit(THandle th)
{
    Errors res = startEdit(th);
    if (res != OK) throw res;
}

void
finish_edit(THandle th)
{
    Errors res = finishEdit(th);
    if (res != OK) throw res;
}

void
put_text(THandle th, const string &fn, const string &val)
{
    Errors res = putText(th, fn.c_str(), val.c_str());
    if (res != OK) throw res;
}
void
put_long(THandle th, const string &fn, long val)
{
    Errors res = putLong(th, fn.c_str(), val);
    if (res != OK) throw res;
}

void
create_rec(THandle th)
{
    Errors res = createNew(th);
    if (res != OK) throw res;   
}

void
delete_rec(THandle th)
{
    Errors res = deleteRec(th);
    if (res != OK) throw res;
}

void
put_text_new(THandle th, const string &fn, const string &val)
{
    Errors res = putTextNew(th, fn.c_str(), val.c_str());
    if (res != OK) throw res;
}

void
put_long_new(THandle th, const string &fn, long val)
{
    Errors res = putLongNew(th, fn.c_str(), val);
    if (res != OK) throw res;
}

void
insert(THandle th)
{
    Errors res = insertNew(th);
    if (res != OK) throw res;
}

void
insert_begin(THandle th)
{
    Errors res = insertaNew(th);
    if (res != OK) throw res;
}

void insert_end(THandle th)
{
    Errors res = insertzNew(th);
    if (res != OK) throw res;
}

unsigned
get_field_len(THandle th, const string &fn)
{
    unsigned ret;
    Errors res = getFieldLen(th, fn.c_str(), &ret);
    if (res != OK) throw res;
    return ret;
}

unsigned
get_field_len(THandle th, unsigned idx)
{
    char *name;
    Errors res = getFieldName(th, idx, &name);
    if (res != OK) throw res;
    unsigned ret;
    res = getFieldLen(th, name, &ret);
    if (res != OK) throw res;
    return ret;
}

FieldType
get_field_type(THandle th, const string &fn)
{
    FieldType ret;
    Errors res = getFieldType(th, fn.c_str(), &ret);
    if (res != OK) throw res;
    return ret;
}

FieldType
get_field_type(THandle th, unsigned idx)
{
    char *buf;
    Errors res = getFieldName(th, idx, &buf);
    if (res != OK) throw res;
    FieldType ret;
    res = getFieldType(th, buf, &ret);
    if (res != OK) throw res;
    return ret;
}

unsigned
get_field_num(THandle th)
{
    unsigned ret;
    Errors res = getFieldsNum(th, &ret);
    if (res != OK) throw res;
    return ret;
}

string
get_field_name(THandle th, unsigned idx)
{
    char *buf;
    Errors res = getFieldName(th, idx, &buf);
    if (res != OK) throw res;
    return string(buf);
}

void print_table(THandle th)
{
    move_first(th);
    unsigned num = get_field_num(th);
    std::cout << "Table:" << std::endl;
    while (!end(th)) {
        for (unsigned i = 0; i < num; ++i) {
            string name = get_field_name(th, i);
            FieldType type = get_field_type(th, name);
            if (type == Long) std::cout << get_long(th, name) << "   ";
            else if (type == Text) std::cout << get_text(th, name) << "   ";
            else std::cout << "no TEXT no LONG   ";
        }
        std::cout << std::endl;
        move_next(th);
    }
    std::cout << std::endl;
    
}

bool isfield(THandle th, const string &field, size_t &id)
{
    for (size_t i = 0; i < get_field_num(th); ++i) {
        if (get_field_name(th, i) == field) {
            id = i;
            return true;
        }
    }
    return false;
}

bool isfield(THandle th, const string &field)
{
    for (size_t i = 0; i < get_field_num(th); ++i) {
        if (get_field_name(th, i) == field) {
            return true;
        }
    }
    return false;
}
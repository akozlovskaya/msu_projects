#ifndef __RDBMSTABLE__
#define __RDBMSTABLE__

#include <iostream>
#include <string>
#include <map>

using std::map;
using std::string;

extern "C" {
    #include "table.h"
}

bool isopened(const string &, THandle &);

void create_table(const string &, struct TableStruct *);
void delete_table(const string &);
THandle open(const string &);
void close(THandle);

void move_first(THandle);
void move_last(THandle);
void move_next(THandle);
void move_prev(THandle);

bool begin(THandle);
bool end(THandle);

string get_text(THandle, const string &);
long get_long(THandle, const string &);

string get_text(THandle, unsigned);
long get_long(THandle, unsigned);

void edit(THandle);
void finish_edit(THandle);

void put_text(THandle, const string &, const string &);
void put_long(THandle, const string &, long);

void create_rec(THandle);
void delete_rec(THandle);

void put_text_new(THandle, const string &, const string &);
void put_long_new(THandle, const string &, long);

void insert(THandle);
void insert_begin(THandle);
void insert_end(THandle);

unsigned get_field_len(THandle, const string &);
unsigned get_field_len(THandle, unsigned);
FieldType get_field_type(THandle, const string &);
FieldType get_field_type(THandle, unsigned);
unsigned get_field_num(THandle);
string get_field_name(THandle, unsigned);

bool isfield(THandle, const string &, size_t &);
bool isfield(THandle, const string &);

void print_table(THandle);

#endif /* __RDBMSTABLE__ */

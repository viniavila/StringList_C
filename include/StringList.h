#ifndef STRINGLIST_H
#define STRINGLIST_H

#include <unistd.h>
#include <stdarg.h>

struct String_;
typedef struct String_ String;

String * String_new(const char * chr);
String * StringBuilder(size_t c, ...);
String * String_delete(String * str);

size_t String_size(String * str);
char const * const String_cstr(String * str);
void String_append_char(String * str, const char c);
void String_append_cstr(String * str, const char * c);
void String_append_string(String * str, String * app_str);

int String_index_of(String * str, const char * str_find, size_t initial_position);
int String_contains(String * str, const char * str_find);
int String_compare(String * str1, String * str2);

struct StringList_;
typedef struct StringList_ StringList;

StringList * StringList_new(size_t c);
StringList * StringList_new_from_list(size_t c, ...);
StringList * StringList_split(String * str, const char * sep);
StringList * StringList_delete(StringList * strlist);

size_t StringList_size(StringList * strlist);
String * StringList_at(StringList * strlist, size_t index);
int StringList_index_of(StringList * strlist, const char * str, size_t initial_position);

void StringList_set_value(StringList * strlist, size_t index, const char * str);

void StringList_append(StringList * strlist, const char * str);
void StringList_prepend(StringList * strlist, const char * str);
void StringList_insert(StringList * strlist, size_t index, const char * str);

String * StringList_take_at(StringList * strlist, size_t index);
void StringList_remove_at(StringList * strlist, size_t index);
void StringList_remove_one(StringList * strlist, const char * str);
void StringList_remove_all(StringList * strlist, const char * str);
void StringList_clear(StringList * strlist);

void StringList_replace_at(StringList * strlist, size_t index, const char * new_str);
String * StringList_join(StringList * strlist, char * sep);

typedef String** StringListIterator;
String * StringList_first(StringList * strlist);
String * StringList_last(StringList * strlist);
StringListIterator StringList_begin(StringList * strlist);
StringListIterator StringList_end(StringList * strlist);


#endif // STRINGLIST_H

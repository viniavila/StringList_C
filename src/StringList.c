#include "StringList.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

// Defining Boolean constants and type (C99 does not have native support for bool type) - Not necessary for this module but still funny :-)
#define TRUE 1
#define FALSE 0
typedef unsigned short bool_t;

struct String_ {
    size_t size;
    char * data;
};

struct StringList_ {
    size_t size;
    String ** data;
};

String * String_new(const char * chr) {
    String * str = malloc(sizeof(String));
    if (chr != NULL) {
        size_t size = strlen(chr);
        str->size = size;
        str->data = strdup(chr);
    }
    else {
        str->size = 0;
        str->data = NULL;
    }
    return str;
}

String * StringBuilder(size_t c, ...) {
    String * str = malloc(sizeof(String));
    str->size = 0;
    va_list vl;
    va_start(vl, c);
    for (size_t i = 0; i < c; ++i) {
        char * tmp = va_arg(vl, char*);
        str->size += strlen(tmp);
    }
    va_end(vl);
    str->data = malloc(sizeof(char) * (str->size+1));
    str->data[str->size] = '\0';
    char * p = str->data;
    va_start(vl, c);
    for (size_t i = 0; i < c; ++i) {
        char * tmp = va_arg(vl, char*);
        size_t tmp_t = strlen(tmp);
        memcpy(p, tmp, tmp_t);
        p += tmp_t;
    }
    va_end(vl);
    return str;
}

String * String_delete(String * str) {
    if (str->data != NULL) free(str->data);
    free(str);
    return NULL;
}

size_t String_size(String * str) {
    return str->size;
}

char const * const String_cstr(String * str) {
    return (char const * const)str->data;
}

void String_append_char(String * str, const char c) {
    char * tmp = malloc(sizeof(char) * (str->size+2));
    if (str->data != NULL) memcpy(tmp, str->data, str->size);
    tmp[str->size] = c;
    tmp[str->size+1] = '\0';
    str->size++;
    if (str->data != NULL) free(str->data);
    str->data = tmp;
}

void String_append_cstr(String * str, const char * c) {
    size_t c_t = strlen(c);
    char * tmp = malloc(sizeof(char) * (str->size+1+c_t));
    if (str->data != NULL) memcpy(tmp, str->data, str->size);
    memcpy(tmp+str->size, c, c_t);
    tmp[str->size+c_t] = '\0';
    str->size += c_t;
    if (str->data != NULL) free(str->data);
    str->data = tmp;
}

void String_append_string(String * str, String * app_str) {
    size_t app_t = app_str->size;
    char * app_cstr = app_str->data;
    char * tmp = malloc(sizeof(char) * (str->size+1+app_t));
    if (str->data != NULL) memcpy(tmp, str->data, str->size);
    memcpy(tmp+str->size, app_cstr, app_t);
    tmp[str->size+app_t] = '\0';
    str->size += app_t;
    if (str->data != NULL) free(str->data);
    str->data = tmp;
}

void String_replace_char(String * str, size_t index, const char new_char) {
    assert(index >= 0 && index < str->size);
    str->data[index] = new_char;
}

int String_index_of(String * str, const char * str_find, size_t initial_position) {
    assert(initial_position >= 0 && initial_position < str->size - 1);
    size_t str_t = strlen(str_find);
    char * pd = str->data+initial_position;
    char buf[str_t+1];
    buf[str_t] = '\0';
    for (size_t i = initial_position; i < str->size; ++i) {
        memcpy(buf, pd, str_t);
        if (strcmp(buf, str_find) == 0)
            return i;
        pd++;
    }
    return -1;
}

int String_contains(String * str, const char * str_find) {
    int p = String_index_of(str, str_find, 0);
    return (p >= 0);
}

int String_compare(String * str1, String * str2) {
    return strcmp(str1->data, str2->data);
}

StringList * StringList_new(size_t c) {
    StringList * strlist = malloc(sizeof(StringList));
    strlist->size = c;
    if (c == 0) {
        strlist->data = malloc(sizeof(String*));
        strlist->data[0] = NULL;
    }
    else {
        strlist->data = malloc(sizeof(String*)*(c+1));
        for (size_t i = 0; i <= c; ++i)
            strlist->data[i] = NULL;
    }
    return strlist;
}

StringList * StringList_new_from_list(size_t c, ...) {
    assert(c > 0);
    StringList * strlist = malloc(sizeof(StringList));
    strlist->size = c;
    strlist->data = malloc(sizeof(String*)*(c+1));
    va_list vl;
    va_start(vl, c);
    String ** pD = strlist->data;
    for (size_t i = 0; i < c; ++i, ++pD) {
        String * string = String_new(va_arg(vl, char*));
        *pD = string;
    }
    strlist->data[c] = NULL;
    va_end(vl);
    return strlist;
}

StringList * StringList_split(String * str, const char * sep) {
    size_t strl_t = 1;
    int pos = -1;
    do {
        pos = String_index_of(str, sep, pos+1);
        if (pos >=0)
            strl_t++;
    } while (pos >= 0 && pos < str->size-1);
    StringList * strlist = StringList_new(strl_t);
    if (strl_t == 1)
        strlist->data[0] = String_new(str->data);
    else {
        size_t index = 0;
        int begin_p = 0;
        int end_p;
        do {
            if (begin_p == str->size) {
                strlist->data[index] = String_new("");
                end_p = -1;
            }
            else {
                size_t sep_t = strlen(sep);
                size_t item_t;
                end_p = String_index_of(str, sep, begin_p);
                if (end_p >= 0)
                    item_t = end_p - begin_p;
                else
                    item_t = str->size - begin_p;
                char * tmp = malloc(sizeof(char) * (item_t+1));
                tmp[item_t] = '\0';
                memcpy(tmp, str->data+begin_p, item_t);
                String * item = String_new(NULL);
                item->size = item_t;
                item->data = tmp;
                strlist->data[index] = item;
                index++;
                begin_p += item_t + sep_t;
            }
        } while (end_p >= 0);
    }
    return strlist;
}

StringList * StringList_delete(StringList * strlist) {
    if (strlist->data != NULL) {
        for (size_t i = 0; i < strlist->size; ++i)
            if (strlist->data[i] != NULL) String_delete(strlist->data[i]);
        free(strlist->data);
    }
    free(strlist);
    return NULL;
}


size_t StringList_size(StringList * strlist) {
    return strlist->size;
}

String * StringList_at(StringList * strlist, size_t index) {
    assert(index >= 0 && index < strlist->size);
    return strlist->data[index];
}

int StringList_index_of(StringList * strlist, const char * str, size_t initial_position) {
    assert(initial_position >= 0 && initial_position < strlist->size - 1);
    for (size_t i = initial_position; i < strlist->size; ++i) {
        if (strcmp(String_cstr(strlist->data[i]), str)==0)
            return i;
    }
    return -1;
}

void StringList_set_value(StringList * strlist, size_t index, const char * str) {
    assert(index >= 0 && index < strlist->size);
    if (strlist->data[index] != NULL) String_delete(strlist->data[index]);
    strlist->data[index] = String_new(str);
}


void StringList_append(StringList * strlist, const char * str) {
    StringList_insert(strlist, strlist->size, str);
}

void StringList_prepend(StringList * strlist, const char * str) {
    StringList_insert(strlist, 0, str);
}

void StringList_insert(StringList * strlist, size_t index, const char * str) {
    assert(index >=0 && index <= strlist->size);
    size_t new_size_t = strlist->size+1;
    String ** tmp = malloc(sizeof(String*) * (new_size_t+1));
    if (index > 0)
        memcpy(tmp, strlist->data, sizeof(String*)*index);
    if (index < strlist->size)
        memcpy(tmp+index+1, strlist->data+index, sizeof(String*)*(strlist->size - index));
    tmp[index] = String_new(str);
    tmp[new_size_t] = NULL;
    free(strlist->data);
    strlist->data = tmp;
    strlist->size = new_size_t;
}

String * StringList_take_at(StringList * strlist, size_t index) {
    assert(strlist->size > 0 && index >= 0 && index < strlist->size);
    size_t new_size_t = strlist->size-1;
    String * str_taken = strlist->data[index];
    if (new_size_t == 0) {
        free(strlist->data);
        strlist->data = NULL;
        strlist->size = 0;
    }
    else {
        String ** tmp = malloc(sizeof(String*) * (new_size_t+1));
        if (index > 0)
            memcpy(tmp, strlist->data, sizeof(String*)*index);
        if (index < strlist->size-1)
            memcpy(tmp+index, strlist->data+index+1, sizeof(String*)*(new_size_t - index));
        free(strlist->data);
        strlist->data = tmp;
        strlist->size = new_size_t;
    }
    return str_taken;
}

void StringList_remove_at(StringList * strlist, size_t index) {
    String * str = StringList_take_at(strlist, index);
    String_delete(str);
}

void StringList_remove_one(StringList * strlist, const char * str) {
    int index = StringList_index_of(strlist, str, 0);
    if (index >= 0)
        StringList_remove_at(strlist, index);
}

void StringList_remove_all(StringList * strlist, const char * str) {
    int index = 0;
    do {
        index = StringList_index_of(strlist, str, index);
        if (index >= 0)
            StringList_remove_at(strlist, index);
    } while (index >= 0);
}

void StringList_clear(StringList * strlist) {
    if (strlist->data != NULL) {
        for (size_t i = 0; i <= strlist->size; ++i)
            if (strlist->data[i] != NULL) String_delete(strlist->data[i]);
        free(strlist->data);
    }
    strlist->size = 0;
    strlist->data = malloc(sizeof(String*));
    strlist->data = NULL;
}

void StringList_replace_at(StringList * strlist, size_t index, const char * new_str) {
    assert(index >= 0 && index < strlist->size);
    String * old_str = strlist->data[index];
    strlist->data[index] = String_new(new_str);
    String_delete(old_str);
}

String * StringList_join(StringList * strlist, char * sep) {
    size_t str_t = 0;
    size_t sep_t = strlen(sep);
    for (int i = 0; i < strlist->size; ++i)
        str_t += strlist->data[i]->size;
    str_t += (strlist->size-1) * sep_t;
    char * tmp = malloc(sizeof(char) * (str_t+1));
    tmp[str_t] = '\0';
    char * pTMP = tmp;
    for (int i=0; i < strlist->size; ++i) {
        char * item = strlist->data[i]->data;
        size_t item_t = strlen(item);
        memcpy(pTMP, item, item_t);
        pTMP += item_t;
        if (i < strlist->size-1) {
            memcpy(pTMP, sep, sep_t);
            pTMP += sep_t;
        }
    }
    return String_new(tmp);
}

String * StringList_first(StringList * strlist) {
    return strlist->data[0];
}

String * StringList_last(StringList * strlist) {
    return strlist->data[strlist->size-1];
}

StringListIterator StringList_begin(StringList * strlist) {
    return strlist->data;
}

StringListIterator StringList_end(StringList * strlist) {
    return strlist->data+strlist->size;
}

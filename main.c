#include <stdio.h>
#include "StringList.h"

int main()
{
    String * str = String_new("Vinicius de Avila Jorge");
    StringList * strl = StringList_split(str, " ");
    StringList_prepend(strl, "Eng.");
    StringList_remove_at(strl, 2);
    StringList_replace_at(strl, 2, "A.");
    StringList_append(strl, "- MBA");
    String * str2 = StringList_join(strl, " ");
    for (StringListIterator iter = StringList_begin(strl); iter != StringList_end(strl); iter++) {
        printf("%s\n", String_cstr(*iter));
    }

    String_delete(str);
    String_delete(str2);
    StringList_delete(strl);
    return 0;
}

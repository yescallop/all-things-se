#include <cstring>
#include <iostream>

void insert_str(char main_str[], char sub_str[], int pos) {
    int main_len = strlen(main_str);
    int sub_len = strlen(sub_str);
    memmove(main_str + pos + sub_len, main_str + pos, main_len - pos + 1);
    memcpy(main_str + pos, sub_str, sub_len);
}

int main() {
    char main_str[100] = "hello, cat!\n";
    char sub_str[100] = " and dog";
    insert_str(main_str, sub_str, 10);

    printf("%s", main_str);

    return 0;
}

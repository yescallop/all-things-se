#include <cstdio>

int main() {
    char buf[64];
    unsigned long long num;
    while (scanf("%63[^0-9]", buf) >= 0) {
        scanf("%u", &num);
        printf("%u ", num);
    }
    printf("\n");
}
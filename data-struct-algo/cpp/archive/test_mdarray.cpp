#include <cstdint>
#include <cstdio>
#include <type_traits>

int main() {
    int arr[2][3] = {{1, 2, 3}, {4, 5, 6}};

    int(*good1)[3] = arr;
    int *good2 = (int *)arr;

    printf("arr: %p\n", arr);
    printf("arr + 1: %p\n", arr + 1);
    printf("alignof(int): %u\n", alignof(int));
    printf("alignof(int *): %u\n", alignof(int *));
    printf("uintptr_t(arr + 1) %% alignof(int *): %u\n",
           uintptr_t(arr + 1) % alignof(int *));

    // undefined behavior
    int **bad = (int **)(arr + 1);
    printf("bad: %p\n", bad);
    printf("(int *) bad: %p\n", (int *)bad);

    return 0;
}
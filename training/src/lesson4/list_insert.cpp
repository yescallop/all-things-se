#include <algorithm>
#include <iostream>

void insert_data(int a[], int n, int x) {
    int *i;
    if (n >= 2 && a[0] > a[1]) {
        i = std::upper_bound(a, a + n, x, std::greater<>());
    } else {
        i = std::upper_bound(a, a + n, x);
    }
    for (int *p = a + n; p > i; p--) {
        *p = *(p - 1);
    }
    *i = x;
}

int main() {
    int a[6] = {1,2,3,4,5};
    insert_data(a, 5, 4);
    for (int i : a) {
        printf("%d\n", i);
    }
    return 0;
}

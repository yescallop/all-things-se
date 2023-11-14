#include <algorithm>
#include <cstdio>
#include <cstring>
#include <utility>

typedef unsigned long long u64;

static u64 inverse_pairs = 0;

void merge(u64 *a1, u64 *a2, u64 *b1, u64 *b2, u64 *c) {
    while (a1 != a2 && b1 != b2) {
        if (*a1 <= *b1)
            *(c++) = *(a1++);
        else
            *(c++) = *(b1++), inverse_pairs += a2 - a1;
    }
    if (a1 != a2)
        memcpy(c, a1, (a2 - a1) * sizeof(u64));
    if (b1 != b2)
        memcpy(c, b1, (b2 - b1) * sizeof(u64));
}

u64 *merge_sort(u64 *arr, size_t n) {
    u64 *tmp = new u64[n];
    for (size_t size = 1; size < n; size *= 2) {
        for (size_t left = 0; left < n; left += size * 2) {
            size_t mid = std::min(left + size, n);
            size_t right = std::min(mid + size, n);
            merge(arr + left, arr + mid, arr + mid, arr + right, tmp + left);
        }
        std::swap(arr, tmp);
    }
    delete[] tmp;
    return arr;
}

int main() {
    u64 n;
    scanf("%llu", &n);

    u64 *arr = new u64[n];
    for (u64 i = 0; i < n; i++) {
        scanf("%llu", &arr[i]);
    }
    arr = merge_sort(arr, n);
    printf("%llu\n", inverse_pairs);

    delete arr;
    return 0;
}
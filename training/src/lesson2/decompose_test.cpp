#include <iostream>
#include <vector>

typedef unsigned long long u64;

u64 int_sqrt(u64 s) {
    if (s <= 1)
        return s;
    u64 x0 = s / 2;
    u64 x1 = (x0 + s / x0) / 2;
    while (x1 < x0) {
        x0 = x1;
        x1 = (x0 + s / x0) / 2;
    }
    return x0;
}

int main() {
    std::cout << "n: ";
    u64 n;
    std::cin >> n;

    u64 max = int_sqrt(n);

    std::vector<bool> is_composite(max + 1, false);

    std::cout << n << "=";

    u64 i = 1;
    while (i < max) {
        i += 1;
        if (is_composite[i])
            continue;
        for (u64 j = i * 2; j <= max; j += i) {
            is_composite[j] = true;
        }
        while (n % i == 0) {
            n /= i;
            std::cout << i;
            if (n != 1)
                std::cout << '*';
        }
        max = int_sqrt(n);
    }
    if (n != 1)
        std::cout << n;
    std::cout << '\n';
    return 0;
}
#include <iostream>

void count_digit(long long n, int a[]) {
    if (n == 0) {
        a[0] = 1;
        return;
    }

    unsigned long long x = std::abs(n);

    while (x != 0) {
        a[x % 10] += 1;
        x /= 10;
    }
}

int main() {
    std::cout << "n: ";
    long long n;
    std::cin >> n;

    int a[10] = {};
    count_digit(n, a);

    for (int i = 0; i < 10; i++)
        std::cout << i << '*' << a[i] << ' ';
    std::cout << '\n';

    return 0;
}

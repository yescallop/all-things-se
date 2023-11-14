#include <iostream>

int main() {
    std::cout << "input: ";
    int n;
    std::cin >> n;

    std::cout << n << "^3=";
    int start = n * (n - 1) + 1;
    for (int i = 0; i < n; i++) {
        if (i != 0)
            std::cout << '+';
        int t = start + i * 2;
        std::cout << t;
    }
    std::cout << '\n';
    return 0;
}
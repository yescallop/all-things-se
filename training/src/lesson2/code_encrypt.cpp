#include <iostream>

int main() {
    int data;
    std::cin >> data;
    if (data < 1000 || data >= 10000) {
        std::cout << "invalid data!\n";
        return 0;
    }

    char digits[4];
    for (size_t i = 0; i < 4; i++) {
        digits[i] = (data + 5) % 10;
        data /= 10;
    }

    int res = digits[0] * 1000 + digits[1] * 100 + digits[2] * 10 + digits[3];
    std::cout << res << '\n';

    return 0;
}
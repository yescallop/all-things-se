#include <iostream>

int main() {
    for (int i = 0; i < 100; i++) {
        int n = i * i;
        int l = n / 100, r = n % 100;
        if (l % 11 == 0 && r % 11 == 0 && l != r) {
            std::cout << "plate number: " << n << '\n';
        }
    }
    return 0;
}
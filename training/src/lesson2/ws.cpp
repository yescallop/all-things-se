#include <iostream>

int main() {
    for (int i = 2; i < 1000; i++) {
        int sum = 0;
        for (int j = 1; j <= i / 2; j++) {
            if (i % j == 0) {
                sum += j;
            }
        }
        if (i == sum) {
            std::cout << i << '\n';
        }
    }
    return 0;
}
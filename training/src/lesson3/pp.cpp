#include <iostream>

int main() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (i != j && j != k && i != k && i != 0 && j != 0 && j != 2) {
                    std::cout << "A: " << char('X' + i) << '\n';
                    std::cout << "B: " << char('X' + j) << '\n';
                    std::cout << "C: " << char('X' + k) << '\n';
                }
            }
        }
    }

    return 0;
}
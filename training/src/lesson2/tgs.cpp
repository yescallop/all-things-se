#include <iostream>

int main() {
    int next_power_of_ten = 10;
    for (int i = 1; i <= 10000; i++) {
        if (i >= next_power_of_ten) {
            next_power_of_ten *= 10;
        }
        int square = i * i;
        if (square % next_power_of_ten == i) {
            std::cout << i << '\n';
        }
    }
    return 0;
}

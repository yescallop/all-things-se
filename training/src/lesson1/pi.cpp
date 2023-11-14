#include <format>
#include <iostream>

int main() {
    double sum = 0.0;
    int i = 1;
    double diff;

    do {
        diff = 1 / double(i * 2 - 1);
        sum += diff * double((i & 1) * 2 - 1);
        i += 1;
    } while (diff > 1e-8);

    std::cout << std::format("estimate of pi: {:.8f}\n", sum * 4);

    return 0;
}
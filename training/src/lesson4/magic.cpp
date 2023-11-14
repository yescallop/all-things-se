#include <iostream>

int matrix[100][100];

int main() {
    std::cout << "n: ";
    int n;
    std::cin >> n;

    if (n < 1 || n > 100)
        throw std::out_of_range("n must be between 1 and 100");
    if (n % 2 == 0)
        throw std::out_of_range("n must be odd");

    int x = n / 2, y = 0;

    for (int i = 1; i <= n * n; i++) {
        matrix[y][x] = i;
        if (i % n != 0) {
            x = (x + 1) % n;
            y = (y - 1 + n) % n;
        } else {
            y = (y + 1) % n;
        }
    }

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            std::cout << matrix[y][x] << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}
#include <iostream>

int main() {
    std::cout << "n: ";
    int n;
    std::cin >> n;

    if (n < 2 || n > 9)
        throw std::out_of_range("n must be between 2 and 9");

    std::cout << "array: ";

    int arr[9];
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    int matrix[9][9];
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            matrix[y][x] = arr[(y + x) % n];
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
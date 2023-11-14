#include <iostream>

int main() {
    std::cout << "n: ";
    int n;
    std::cin >> n;

    for (int i = 0; i < n * 2 - 1; i++) {
        int cnt = (n - abs(i - n + 1)) * 2 - 1;
        for (int j = 0; j < cnt; j++)
            std::cout << '*';
        std::cout << '\n';
    }
    return 0;
}

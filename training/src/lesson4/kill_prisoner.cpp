#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::cout << "n: ";
    int n;
    std::cin >> n;

    std::vector<int> prisoners(n);
    std::iota(prisoners.begin(), prisoners.end(), 1);
    int remaining = n;

    while (remaining > 0) {
        int cnt = 0;
        for (int &p : prisoners) {
            if (p != 0) {
                cnt++;
                if (cnt % 2 == 1) {
                    remaining -= 1;
                    if (remaining == 0)
                        std::cout << p << '\n';
                    p = 0;
                }
            }
        }
    }

    return 0;
}
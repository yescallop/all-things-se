#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::cout << "n: ";
    int n;
    std::cin >> n;

    if (n < 2)
        throw std::out_of_range("n must be >= 2");

    std::cout << "height: ";

    std::vector<int> height;
    for (int i = 0; i < n; i++) {
        int h;
        std::cin >> h;
        height.push_back(h);
    }

    int max_v = 0;

    for (int r = 1; r < n; r++) {
        int rh = height[r];
        for (int l = 0; l < r; l++) {
            int lh = height[l];
            int v = (r - l) * std::min(rh, lh);
            if (v > max_v)
                max_v = v;
        }
    }

    std::cout << "max volume: " << max_v << '\n';

    return 0;
}
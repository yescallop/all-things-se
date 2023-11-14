#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <random>

int main() {
    std::array<int, 100> nums;
    std::iota(nums.begin(), nums.end(), 2020001);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(nums.begin(), nums.end(), g);

    std::cout << "A:" << '\n';
    for (int i = 0; i < 50; i++) {
        std::cout << nums[i] << ' ';
        if (i % 8 == 7)
            std::cout << '\n';
    }
    std::cout << '\n';
    std::cout << "B:" << '\n';
    for (int i = 0; i < 50; i++) {
        std::cout << nums[i + 50] << ' ';
        if (i % 8 == 7)
            std::cout << '\n';
    }
    std::cout << '\n';
    return 0;
}
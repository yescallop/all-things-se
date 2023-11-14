#include <algorithm>
#include <iostream>

int main() {
    std::cout << "input: ";
    std::string s;
    std::cin >> s;

    auto l = s.begin(), r = s.end();
    bool is_num = !s.empty() && std::all_of(l, r, [](unsigned char c) {
        return std::isdigit(c);
    });

    if (!is_num) {
        std::cout << "input is not a number\n";
        return 0;
    }

    r -= 1;
    while (true) {
        if (*l != *r) {
            std::cout << "input is not a palindrome\n";
            return 0;
        }
        if (r - l < 2)
            break;
        l += 1;
        r -= 1;
    }

    std::cout << "input is a palindrome\n";
    return 0;
}
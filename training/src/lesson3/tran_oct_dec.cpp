#include <iostream>

int main() {
    std::cout << "input: ";
    std::string s;
    std::cin >> s;

    unsigned long long n = 0;

    for (char ch : s) {
        if (ch < '0' || ch > '7') {
            std::cout << "not an octal number\n";
            return 1;
        }
        n = (n << 3) | (ch - '0');
    }

    std::cout << n << '\n';

    return 0;
}
#include <iostream>

int main() {
    std::cout << "input: ";

    std::string s;
    std::cin >> s;

    int cnt[256] = {};
    for (unsigned char ch : s)
        cnt[ch] += 1;

    for (int i = 0; i < 256; i++) {
        if (cnt[i] != 0)
            std::cout << char(i) << '=' << cnt[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::string text, pat;
    std::cin >> text >> pat;

    size_t plen = pat.size();
    std::vector<size_t> next(plen);
    next[0] = 0;

    for (size_t i = 1, j = 0; i < plen; i++) {
        while (j && pat[i] != pat[j])
            j = next[j - 1];
        next[i] = pat[i] == pat[j] ? ++j : 0;
    }

    for (size_t i = 0, j = 0; i < text.size(); i++) {
        while (j && text[i] != pat[j])
            j = next[j - 1];
        j += text[i] == pat[j];
        if (j == plen)
            std::cout << i + 2 - plen << '\n';
    }

    for (size_t n : next)
        std::cout << n << ' ';
    std::cout << '\n';

    return 0;
}
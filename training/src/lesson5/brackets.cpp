#include <iostream>
#include <string>
#include <vector>

int main() {
    std::cout << "input: ";
    std::string s;
    std::cin >> s;

    std::vector<char> brackets;

    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            brackets.push_back(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (brackets.empty()) {
                std::cout << "No\n";
                return 0;
            }
            char lb = brackets.back();
            if (lb + (lb >> 6) + 1 != c) {
                std::cout << "No\n";
                return 0;
            }
            brackets.pop_back();
        }
    }
    std::cout << "Yes\n";
    return 0;
}
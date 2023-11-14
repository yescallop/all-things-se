#include <iostream>
#include <string>
#include <vector>

enum Code { Match, NotMatch, NoRightMatch, NoLeftMatch };

int match(const std::string &expr, std::vector<char> stack) {
    stack.clear();
    for (char ch : expr) {
        if (ch == '(' || ch == '[' || ch == '{') {
            stack.push_back(ch);
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (stack.empty())
                return Code::NoLeftMatch;
            if (ch != stack.back() + (ch >> 6) + 1)
                return Code::NotMatch;
            stack.pop_back();
        }
    }
    if (!stack.empty())
        return Code::NoRightMatch;
    return Code::Match;
}

int main() {
    std::vector<char> stack;
    std::string expr;

    int n;
    std::cin >> n;
    std::getline(std::cin, expr);

    for (int i = 0; i < n; i++) {
        std::getline(std::cin, expr);
        std::cout << match(expr, stack) << '\n';
    }

    return 0;
}
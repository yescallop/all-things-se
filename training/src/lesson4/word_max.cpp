#include <iostream>
#include <ranges>

int main() {
    std::cout << "input: ";
    std::string line;
    std::getline(std::cin, line);

    std::string_view max = {};

    for (const auto range : std::views::split(line, ' ')) {
        std::string_view word(range.begin(), range.end());
        if (word.size() > max.size()) {
            max = word;
        }
    }

    std::cout << "max: " << max << '\n';
    return 0;
}
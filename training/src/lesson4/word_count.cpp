#include <functional>
#include <iostream>
#include <map>
#include <ranges>

void for_each_word(std::string_view line,
                   std::function<void(std::string_view)> f) {
    while (!line.empty()) {
        size_t i = line.find_first_not_of(" ,.");
        if (i == std::string_view::npos)
            break;
        line.remove_prefix(i);

        i = line.find_first_of(" ,.");
        if (i == std::string_view::npos)
            i = line.size();
        f(line.substr(0, i));
        line.remove_prefix(i);
    }
}

int main() {
    std::cout << "input: ";
    std::string line;
    std::getline(std::cin, line);

    std::map<std::string_view, int> words = {};

    for_each_word(
        line, [&](auto word) { words.try_emplace(word).first->second += 1; });

    for (const auto &[word, cnt] : words) {
        std::cout << word << '\t' << cnt << '\n';
    }
    return 0;
}
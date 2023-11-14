#include <format>
#include <iostream>
#include <string>
#include <vector>

int main() {
    int total;
    std::cin >> total;

    std::vector<std::string> males;
    std::vector<std::string> females;

    for (int i = 0; i < total; i++) {
        std::string name;
        std::cin >> name;

        char gender;
        std::cin >> gender;

        if (gender == 'M') {
            males.push_back(std::move(name));
        } else {
            females.push_back(std::move(name));
        }
    }

    if (males.empty() || females.empty())
        throw "There must be at least one male and one female";

    int count;
    std::cin >> count;
    int m = 0, f = 0;

    for (int i = 0; i < count; i++) {
        auto &mname = males[m];
        auto &fname = females[f];
        std::cout << std::format("{} {} {}\n", i, mname, fname);
        m++, f++;
        if (m >= males.size())
            m = 0;
        if (f >= females.size())
            f = 0;
    }

    return 0;
}
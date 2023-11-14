#include <iostream>

int main() {
    int peach = 1;
    for (int i = 0; i < 9; i++) {
        peach = (peach + 1) * 2;
    }
    std::cout << "There were " << peach << " peaches\n";
    return 0;
}
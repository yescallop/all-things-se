#include "common.hpp"

// Construct a file that requires a longest codeword of length 32
// to compress and has the smallest possible size.
int main() {
    std::ofstream os("lucas.bin", ios::binary | ios::trunc);
    os.put(0);
    os.put(1);

    int a = 1, b = 3, ch = 2;
    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < a; j++)
            os.put(ch);

        int t = b;
        b = a + b;
        a = t;
        ch += 1;
    }
}

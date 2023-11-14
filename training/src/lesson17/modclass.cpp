#include <cstdio>
#include <stdexcept>

class ModClass {
    int dataval;

  public:
    ModClass(int v = 0) : dataval(v % 7) {}
    ModClass operator+(const ModClass &x) const {
        return ModClass((dataval + x.dataval) % 7);
    }
    friend ModClass operator*(const ModClass &a, const ModClass &b);
    int GetValue() const { return dataval; }
    operator int() const { return dataval; }
};

ModClass operator*(const ModClass &a, const ModClass &b) {
    return ModClass((a.dataval * b.dataval) % 7);
}

ModClass Inverse(const ModClass &x) {
    int v = x.GetValue();
    if (v == 0)
        throw std::invalid_argument("0 has no inverse");
    for (int i = 1; i <= 6; i++) {
        if ((v * i) % 7 == 1)
            return ModClass(i);
    }
    throw std::logic_error("no inverse");
}

void Solve(ModClass a, ModClass &x, ModClass b) {
    int va = a, vb = b;
    if (va == 0)
        throw std::invalid_argument("a=0");
    for (int i = 0; i <= 6; i++) {
        if ((va * i) % 7 == vb) {
            x = ModClass(i);
            return;
        }
    }
    throw std::logic_error("no solution");
}

int main() {
    ModClass a(10), b(6), x;
    printf("%d+%d=%d\n", a, b, a + b);
    printf("%d*%d=%d\n", a, b, a * b);
    printf("Inverse(%d)=%d\n", a, Inverse(a));
    printf("Inverse(%d)=%d\n", b, Inverse(b));
    Solve(a, x, b);
    printf("%dx=%d => x=%d\n", a, b, x);
    Solve(b, x, a);
    printf("%dx=%d => x=%d\n", b, a, x);
    return 0;
}

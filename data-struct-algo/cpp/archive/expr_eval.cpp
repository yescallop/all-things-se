#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

typedef std::int32_t i32;
typedef std::int64_t i64;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

template <u64 MODULO = (1ull << 32)> class Evaluator {
    static_assert(MODULO > 0 && MODULO <= (1ull << 32));

    std::vector<char> operators = {};
    std::vector<u32> operands = {};
    i32 result = 0;
    bool overflow_flag = false;

  public:
    Evaluator() {}

};

std::optional<u32> eval(std::string_view expr) {
    std::vector<char> ops;
    std::vector<u32> vals;

    for (char ch : expr) {
    }
}

int main() {
    std::cout << "expr: ";
    std::string expr;
    std::getline(std::cin, expr);

    auto val = eval(expr);
    if (!val) {
        std::cerr << "invalid expr\n";
        return 1;
    }

    std::cout << "val ≡ " << *val << " (mod 10000)\n";
    return 0;
}

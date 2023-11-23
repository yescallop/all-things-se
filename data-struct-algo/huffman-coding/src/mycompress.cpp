#include "binary.hpp"
#include "huffman.hpp"

const char *USAGE = "usage: mycompress input_file [output_file]\n";
const char *ERR_FAILED = "error: failed to %s\n";

int run(const fs::path &in_path, const fs::path &out_path) {
    std::ifstream is(in_path, ios::binary);

    if (!is) {
        fprintf(stderr, ERR_FAILED, "open input");
        return 1;
    }

    std::array<u64, 256> weights;
    u64 in_len_bytes = count_bytes(is, weights);

    printf("input length: %" PRIu64 " bytes\n", in_len_bytes);
    for (int byte = 0; byte < 256; byte++) {
        if (weights[byte] != 0)
            printf("weight of %02x: %" PRIu64 "\n", byte, weights[byte]);
    }

    TreeNodePtr tree = build_tree(weights);

    CodeTable table;
    CodeStore store;
    u64 encoded_len_bits = build_code(tree, table, store);

    for (int byte = 0; byte < 256; byte++) {
        auto [len, val] = table[byte];
        if (len != 0) {
            printf("codeword of %02x: ", byte);
            for (int i = 0; i < len; i++) {
                printf("%d", int(val & 1));
                val >>= 1;
            }
            putchar('\n');
        }
    }

    printf("encoded length: %" PRIu64 " bits\n", encoded_len_bits);

    std::ofstream os(out_path, ios::binary | ios::trunc);

    if (!os) {
        fprintf(stderr, ERR_FAILED, "open output");
        return 1;
    }

    BitWriter bw(&os);
    bw.write(store);
    bw.write(is, table, in_len_bytes);
    bw.flush();

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fputs(USAGE, stderr);
        return 1;
    }

    fs::path in_path = argv[1], out_path = argv[argc - 1];
    if (argc == 2)
        out_path += ".huff";

    return run(in_path, out_path);
}

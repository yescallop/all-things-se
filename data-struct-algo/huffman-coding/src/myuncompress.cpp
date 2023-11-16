#include "binary.hpp"
#include "huffman.hpp"

const char *USAGE = "usage: myuncompress input_file [output_file]\n";
const char *ERR_UNKNOWN_EXTENSION = "error: unknown file extension, skipping\n";
const char *ERR_FAILED = "error: failed to %s\n";

int run(const fs::path &in_path, const fs::path &out_path) {
    std::ifstream is(in_path, ios::binary);

    if (!is) {
        fprintf(stderr, ERR_FAILED, "open input");
        return 1;
    }

    BitReader br(std::move(is));

    CodeDict dict;
    if (!br.read(dict)) {
        fprintf(stderr, ERR_FAILED, "read Huffman code");
        return 1;
    }

    TreeNodePtr tree = build_tree(dict);

    u64 msg_len;
    if (!br.read(msg_len, 64)) {
        fprintf(stderr, ERR_FAILED, "read message length");
        return 1;
    }

    std::ofstream os(out_path, ios::binary | ios::trunc);

    if (!os) {
        fprintf(stderr, ERR_FAILED, "open output");
        return 1;
    }

    if (!br.read(os, tree, msg_len)) {
        fprintf(stderr, ERR_FAILED, "read message");
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fputs(USAGE, stderr);
        return 1;
    }

    fs::path in_path = argv[1], out_path = argv[argc - 1];
    if (argc == 2) {
        if (in_path.extension() != ".huff") {
            fprintf(stderr, ERR_UNKNOWN_EXTENSION);
            return 1;
        }
        out_path.replace_extension("");
    }

    return run(in_path, out_path);
}
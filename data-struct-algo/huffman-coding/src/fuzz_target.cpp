// Compiler options:
// clang++ -g -O2 -std=c++20 -fsanitize=fuzzer,address,undefined
#include "binary.hpp"
#include "huffman.hpp"

// https://stackoverflow.com/a/57862858/9133883
struct membuf : std::streambuf {
    membuf(char *begin, char *end) { this->setg(begin, begin, end); }

    pos_type
    seekoff(off_type off, std::ios_base::seekdir dir,
            std::ios_base::openmode which = std::ios_base::in) override {
        if (dir == std::ios_base::cur)
            gbump(off);
        else if (dir == std::ios_base::end)
            setg(eback(), egptr() + off, egptr());
        else if (dir == std::ios_base::beg)
            setg(eback(), eback() + off, egptr());
        return gptr() - eback();
    }

    pos_type seekpos(pos_type sp, std::ios_base::openmode which) override {
        return seekoff(sp - pos_type(off_type(0)), std::ios_base::beg, which);
    }
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    membuf buf((char *)Data, (char *)Data + Size);
    std::istream is(&buf);

    std::stringstream ss;

    std::array<u64, 256> weights;
    u64 in_len_bytes = count_bytes(is, weights);
    TreeNodePtr tree = build_tree(weights);

    CodeTable table;
    CodeStore store;
    build_code(tree, table, store);

    BitWriter bw(&ss);
    bw.write(store);
    bw.write(is, table, in_len_bytes);
    bw.flush();

    ss.seekg(0);

    BitReader br(&ss);

    CodeStore store_read;
    if (!br.read(store_read))
        throw std::logic_error("failed to read store");

    TreeNodePtr tree_read = build_tree(store);

    std::string_view data_view((const char *)Data, Size);

    std::ostringstream os;
    if (!br.read(os, tree_read) || os.view() != data_view)
        throw std::logic_error("wrong msg");

    return 0;
}

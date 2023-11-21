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
    CodeDict dict;
    build_code(tree, table, dict);

    BitWriter bw(&ss);
    bw.write(dict);
    bw.write(in_len_bytes, 64);
    bw.write(is, table);
    bw.flush();

    ss.seekg(0);

    BitReader br(&ss);

    CodeDict dict_read;
    if (!br.read(dict_read))
        throw std::logic_error("failed to read dict");

    TreeNodePtr tree_read = build_tree(dict);

    u64 msg_len;
    if (!br.read(msg_len, 64))
        throw std::logic_error("failed to read msg len");

    std::string_view data_view((const char *)Data, Size);

    std::ostringstream os;
    if (!br.read(os, tree_read, msg_len) || os.view() != data_view)
        throw std::logic_error("wrong msg");

    return 0;
}

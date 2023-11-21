#pragma once

#include "common.hpp"
#include "huffman.hpp"

/// @brief Counts the occurrences of each byte in an input stream.
/// @param is The input stream to read bytes from.
/// @param counters An array of size 256 to store the count of each byte.
/// @return The total number of bytes in the input stream.
u64 count_bytes(std::istream &is, std::array<u64, 256> &counters) {
    counters = {};
    char byte;
    while (is.get(byte))
        counters[u8(byte)]++;

    is.clear();
    u64 len = is.tellg();
    is.seekg(0);
    return len;
}

class BitWriter {
    std::ostream *os;
    u64 buf = 0;
    u8 buf_len = 0;

  public:
    BitWriter(std::ostream *os) : os(os) {}

    void flush() {
        // Write the remaining bits.
        os->write(reinterpret_cast<char *>(&buf), (buf_len + 7) / 8);
    }

    void write(u64 value, u8 count) {
        if (count > 64)
            throw std::invalid_argument("count > 64");

        buf |= shl(value, buf_len);
        buf_len += count;

        if (buf_len >= 64) {
            os->write(reinterpret_cast<char *>(&buf), 8);
            buf_len -= 64;
            buf = shr(value, count - buf_len);
        }
    }

    void write(const CodeDict &dict) {
        write(dict.size(), CODEWORD_LEN_BITS);
        for (auto &[len, bytes] : dict) {
            write(len, CODEWORD_LEN_BITS);
            write(bytes.size(), 8);
            for (u8 byte : bytes)
                write(byte, 8);
        }
    }

    void write(std::istream &is, const CodeTable &table) {
        char byte;
        while (is.get(byte)) {
            auto [len, val] = table[u8(byte)];
            write(val, len);
        }
    }
};

class BitReader {
    std::istream *is;
    u64 buf = 0;
    u8 buf_len = 0;

  public:
    BitReader(std::istream *is) : is(is) {}

    bool read(u64 &value, u8 count) {
        if (count > 64)
            throw std::invalid_argument("count > 64");

        if (count <= buf_len) {
            value = buf & (shl(1, count) - 1);
        } else {
            u8 lo_len = buf_len;
            u64 lo = buf & (shl(1, lo_len) - 1);
            count -= lo_len;

            is->read(reinterpret_cast<char *>(&buf), 8);
            buf_len = is->gcount() * 8;
            if (count > buf_len)
                return false;

            u64 hi = buf & (shl(1, count) - 1);
            value = shl(hi, lo_len) | lo;
        }

        buf = shr(buf, count);
        buf_len -= count;
        return true;
    }

    bool read(CodeDict &dict) {
        u64 dict_len;
        if (!read(dict_len, CODEWORD_LEN_BITS))
            return false;

        for (int i = 0; i < dict_len; i++) {
            u64 len, bytes_cnt;
            if (!read(len, CODEWORD_LEN_BITS) || !read(bytes_cnt, 8))
                return false;

            auto &bytes = dict.try_emplace(len, bytes_cnt).first->second;
            for (int j = 0; j < bytes_cnt; j++) {
                u64 byte;
                if (!read(byte, 8))
                    return false;
                bytes[j] = u8(byte);
            }
        }
        return true;
    }

    bool read(std::ostream &os, const TreeNodePtr &tree, u64 msg_len) {
        for (u64 i = 0; i < msg_len; i++) {
            TreeNode *cur = tree.get();
            while (cur && (cur->left_child || cur->right_child)) {
                u64 bit;
                if (!read(bit, 1))
                    return false;
                cur = (bit == 0 ? cur->left_child : cur->right_child).get();
            }
            if (!cur)
                return false;
            os.put(cur->byte);
        }
        return true;
    }
};

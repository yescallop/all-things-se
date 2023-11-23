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
        buf = 0;
        buf_len = 0;
    }

    void write(u64 value, u8 count) {
        if (count > 64)
            throw std::invalid_argument("count > 64");

        value = extract_lo(value, count);
        buf |= shl(value, buf_len);
        buf_len += count;

        if (buf_len >= 64) {
            os->write(reinterpret_cast<char *>(&buf), 8);
            buf_len -= 64;
            buf = shr(value, count - buf_len);
        }
    }

    void write(const CodeStore &store) {
        u64 total_bytes = 0;
        for (auto &[len, bytes] : store)
            total_bytes += bytes.size();

        u64 bits_flat = 8 + (CODEWORD_LEN_WIDTH + 8) * total_bytes;
        u64 bits_grouped = CODEWORD_LEN_WIDTH +
                           (CODEWORD_LEN_WIDTH + 8) * store.size() +
                           8 * total_bytes;

        if (bits_flat <= bits_grouped) {
            // Flag as flat.
            write(1, 1);
            // Here total_bytes can be 0 and cannot be 256.
            write(total_bytes, 8);
            for (auto &[len, bytes] : store) {
                for (u8 byte : bytes) {
                    write(len, CODEWORD_LEN_WIDTH);
                    write(byte, 8);
                }
            }
        } else {
            // Flag as grouped.
            write(0, 1);
            write(store.size(), CODEWORD_LEN_WIDTH);
            for (auto &[len, bytes] : store) {
                write(len, CODEWORD_LEN_WIDTH);
                // Here bytes.size() cannot be 0 and can be 256.
                write(bytes.size(), 8);
                for (u8 byte : bytes)
                    write(byte, 8);
            }
        }
    }

    void write(std::istream &is, const CodeTable &table, u64 msg_len) {
        u8 msg_len_width = std::bit_width(msg_len);
        write(msg_len_width, 6);
        write(msg_len, msg_len_width);

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
            value = extract_lo(buf, count);
        } else {
            u8 lo_len = buf_len;
            u64 lo = extract_lo(buf, lo_len);
            count -= lo_len;

            is->read(reinterpret_cast<char *>(&buf), 8);
            buf_len = is->gcount() * 8;
            if (count > buf_len)
                return false;

            u64 hi = extract_lo(buf, count);
            value = shl(hi, lo_len) | lo;
        }

        buf = shr(buf, count);
        buf_len -= count;
        return true;
    }

    bool read(CodeStore &store) {
        u64 flat;
        if (!read(flat, 1))
            return false;
        return flat ? read_flat(store) : read_grouped(store);
    }

    bool read_flat(CodeStore &store) {
        u64 total_bytes;
        if (!read(total_bytes, 8))
            return false;

        for (int i = 0; i < total_bytes; i++) {
            u64 len, byte;
            if (!read(len, CODEWORD_LEN_WIDTH) || !read(byte, 8))
                return false;
            store[len].push_back(u8(byte));
        }
        return true;
    }

    bool read_grouped(CodeStore &store) {
        u64 store_len;
        if (!read(store_len, CODEWORD_LEN_WIDTH))
            return false;

        for (int i = 0; i < store_len; i++) {
            u64 len, bytes_of_len;
            if (!read(len, CODEWORD_LEN_WIDTH) || !read(bytes_of_len, 8))
                return false;
            // Correct the possibly wrapped value.
            if (bytes_of_len == 0)
                bytes_of_len = 256;

            auto &bytes = store.try_emplace(len, bytes_of_len).first->second;
            for (int j = 0; j < bytes_of_len; j++) {
                u64 byte;
                if (!read(byte, 8))
                    return false;
                bytes[j] = u8(byte);
            }
        }
        return true;
    }

    bool read(std::ostream &os, const TreeNodePtr &tree) {
        u64 msg_len_width, msg_len;
        if (!read(msg_len_width, 6) || !read(msg_len, msg_len_width))
            return false;

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

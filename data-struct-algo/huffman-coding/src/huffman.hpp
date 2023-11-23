#pragma once

#include "common.hpp"

using CodewordLen = u8;

// With the maximum length of a Huffman codeword being 63 bits,
// the maximum file length we can handle is at least
// Fibonacci(63+3)-1 bytes = 27,777,890,035,287 B = 25.2 TiB,
// whereas 31 bits allows a mere 5,702,886 B = 5.4 MiB.
//
// See: Y. S. Abu-Mostafa, R. J. McEliece, Maximal codeword lengths
// in Huffman codes, Computers & Mathematics with Applications 39
// (2000), no. 11, 129-134.
const u8 CODEWORD_LEN_BITS = 6;

using CodewordVal = u64;

struct TreeNode;

using TreeNodePtr = std::unique_ptr<TreeNode>;

struct TreeNode {
    u8 byte = 0;
    CodewordLen level = 0;
    u64 weight = 0;

    TreeNodePtr left_child;
    TreeNodePtr right_child;

    TreeNode() {}
    TreeNode(u8 byte, u64 weight) : byte(byte), weight(weight) {}
    TreeNode(TreeNode &&left_child, TreeNode &&right_child)
        : weight(left_child.weight + right_child.weight),
          left_child(new TreeNode(std::move(left_child))),
          right_child(new TreeNode(std::move(right_child))) {}

    bool operator<(const TreeNode &other) const {
        return weight > other.weight;
    }
};

/// Builds a Huffman tree from weights of bytes.
TreeNodePtr build_tree(const std::array<u64, 256> &weights) {
    PriorityQueue<TreeNode> queue;
    for (int byte = 0; byte < 256; byte++)
        if (weights[byte] != 0)
            queue.emplace(byte, weights[byte]);

    // When the input bytes are of the same value,
    // special handling is not needed because they are
    // effectively assigned a codeword of length 0,
    // which is decoded normally just as any other cases.

    while (queue.size() > 1)
        queue.emplace(queue.top_and_pop(), queue.top_and_pop());

    if (queue.empty())
        return nullptr;
    return std::make_unique<TreeNode>(queue.top_and_pop());
}

struct Codeword {
    CodewordLen len = 0;
    CodewordVal val = 0;
};

using CodeTable = std::array<Codeword, 256>;
using CodeStore = std::map<CodewordLen, std::vector<u8>>;

class CanonicalCodeBuilder {
    CodewordLen last_len = 0;
    CodewordVal val_rev = 0;

    static CodewordVal reverse_bits(CodewordVal v, CodewordLen len) {
        CodewordVal res = 0;
        for (int i = 0; i < len; i++) {
            res = (res << 1) | (v & 1);
            v >>= 1;
        }
        return res;
    }

  public:
    CodewordVal next(CodewordLen len) {
        if (last_len != 0)
            val_rev = shl(val_rev + 1, len - last_len);
        last_len = len;
        return reverse_bits(val_rev, len);
    }
};

/// @brief Builds a Huffman code from a Huffman tree.
/// @param tree The input Huffman tree.
/// @param table An array to store the Huffman code.
/// @param store A map to store the Huffman code, sorted by codeword length.
/// @return The encoded message length in bits.
u64 build_code(const TreeNodePtr &tree, CodeTable &table, CodeStore &store) {
    if (!tree)
        return 0;

    std::queue<TreeNode *> queue;
    queue.push(tree.get());

    u64 len = 0;
    CanonicalCodeBuilder cb;

    while (!queue.empty()) {
        TreeNode &node = *queue.front();
        queue.pop();

        if (node.left_child) {
            node.left_child->level = node.level + 1;
            queue.push(node.left_child.get());
        }

        if (node.right_child) {
            node.right_child->level = node.level + 1;
            queue.push(node.right_child.get());
        }

        if (!node.left_child && !node.right_child) {
            Codeword codeword(node.level, cb.next(node.level));
            table[node.byte] = codeword;

            store[codeword.len].push_back(node.byte);

            len += node.weight * codeword.len;
        }
    }
    return len;
}

/// Builds a Huffman tree from a `CodeStore`.
TreeNodePtr build_tree(const CodeStore &store) {
    if (store.empty())
        return nullptr;

    TreeNodePtr tree = std::make_unique<TreeNode>();
    CanonicalCodeBuilder cb;

    for (auto &[len, bytes] : store) {
        for (u8 byte : bytes) {
            TreeNode *cur = tree.get();
            CodewordVal val = cb.next(len);

            for (int i = 0; i < len; i++) {
                if ((val & 1) == 0) {
                    if (!cur->left_child)
                        cur->left_child = std::make_unique<TreeNode>();
                    cur = cur->left_child.get();
                } else {
                    if (!cur->right_child)
                        cur->right_child = std::make_unique<TreeNode>();
                    cur = cur->right_child.get();
                }
                val >>= 1;
            }
            cur->byte = byte;
        }
    }
    return tree;
}

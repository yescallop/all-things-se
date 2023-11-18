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
    CodewordVal codeword_val = 0;

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

    if (queue.size() == 1) {
        TreeNode root(0, queue.top().weight);
        root.left_child = std::make_unique<TreeNode>(queue.top_and_pop());
        queue.emplace(std::move(root));
    }

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

struct CodewordValAndByte {
    CodewordVal codeword_val = 0;
    u8 byte = 0;
};

using CodeDict = std::map<CodewordLen, std::vector<CodewordValAndByte>>;

/// @brief Builds a Huffman code from a Huffman tree.
/// @param tree The input Huffman tree.
/// @param table An array to store the Huffman code.
/// @param dict A map to store the Huffman code, sorted by codeword length.
/// @return The encoded message length in bits.
u64 build_code(const TreeNodePtr &tree, CodeTable &table, CodeDict &dict) {
    if (!tree)
        return 0;

    std::queue<TreeNode *> queue;
    queue.push(tree.get());

    u64 len = 0;
    while (!queue.empty()) {
        TreeNode &node = *queue.front();
        queue.pop();

        if (node.left_child) {
            node.left_child->level = node.level + 1;
            node.left_child->codeword_val = node.codeword_val;
            queue.push(node.left_child.get());
        }

        if (node.right_child) {
            node.right_child->level = node.level + 1;
            node.right_child->codeword_val =
                node.codeword_val | shl(1, node.level);
            queue.push(node.right_child.get());
        }

        if (!node.left_child && !node.right_child) {
            Codeword codeword(node.level, node.codeword_val);
            table[node.byte] = codeword;

            dict[codeword.len].emplace_back(codeword.val, node.byte);

            len += node.weight * codeword.len;
        }
    }
    return len;
}

/// Builds a Huffman tree from a `CodeDict`.
TreeNodePtr build_tree(const CodeDict &dict) {
    if (dict.empty())
        return nullptr;

    TreeNodePtr tree = std::make_unique<TreeNode>();
    for (auto &[len, vec] : dict) {
        for (auto [val, byte] : vec) {
            TreeNode *cur = tree.get();
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

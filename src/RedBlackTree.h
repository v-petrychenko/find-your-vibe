
#pragma once

#include "IndexTree.h"

class RedBlackTree : public IndexTree {

    enum class Color { Red, Black };

    struct Node {
        double key;
        std::vector<int> indices;

        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;

        Color color = Color::Red;

        Node(double k, int  index, Node* p) {
            key = k;
            indices.push_back(index);
            parent = p;
        }
    };

    Node* root = nullptr ;
    std::size_t count = 0;

    static void destroy(Node* node);
    static void rangeHelper(const Node* node, double lo, double hi, std::vector<int>& result);
    static int heightOf(const Node* node);
    static std::size_t indexBytes(const Node* node);
    void rotateLeft(Node* node);
    void rotateRight(Node* node);

    friend void testRotations();

public:
    RedBlackTree();
    ~RedBlackTree() override;

    RedBlackTree(const RedBlackTree&) = delete;
    RedBlackTree& operator=(const RedBlackTree&) = delete;

    void insert(double key, int songIndex) override;
    std::vector<int> find(double key) const override;
    std::vector<int> range(double lo, double hi) const override;
    void clear() override;

    int height() const override;
    std::size_t nodeCount() const override;
    std::size_t memoryBytes() const override;
    std::string name() const override;
};

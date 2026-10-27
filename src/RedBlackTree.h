#pragma once

#include "IndexTree.h"

class RedBlackTree : public IndexTree {
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

private:
};

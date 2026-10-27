#pragma once

#include <cstddef>
#include <string>
#include <vector>

//Interface shared by both RB tree and B-tree
//Each tree maps a key to every song index
class IndexTree {
public:
    virtual ~IndexTree() = default;

    virtual void insert(double key, int songIndex) = 0;
    virtual std::vector<int> find(double key) const = 0;
    virtual std::vector<int> range(double lo, double hi) const = 0;
    virtual void clear() = 0;

    virtual int height() const = 0;
    virtual std::size_t nodeCount() const = 0;
    virtual std::size_t memoryBytes() const = 0;
    virtual std::string name() const = 0;
};

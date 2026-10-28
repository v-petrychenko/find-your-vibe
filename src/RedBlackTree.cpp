
#include "RedBlackTree.h"

using namespace std;

RedBlackTree::RedBlackTree() = default;

RedBlackTree::~RedBlackTree() {
    clear();
}

void RedBlackTree::insert(double key, int songIndex) {
}

vector<int> RedBlackTree::find(double key) const {
    return {};
}

vector<int> RedBlackTree::range(double lo, double hi) const {
    return {};
}

void RedBlackTree::clear() {
}

int RedBlackTree::height() const {
    return 0;
}

size_t RedBlackTree::nodeCount() const {
    return count;
}

size_t RedBlackTree::memoryBytes() const {
    return 0;
}

string RedBlackTree::name() const {
    return "Red-Black Tree";
}

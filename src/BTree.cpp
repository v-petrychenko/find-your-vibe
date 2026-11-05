#include "BTree.h"
#include <algorithm>
using namespace std;

BTree::Node * BTree::makeNode(bool leaf) {
    nodes++;
    return new Node(leaf, t);
}

void BTree::destroy(Node *node) {
    if (node == nullptr) return;
    for (Node* child : node->children) {
        destroy(child);
    }
    delete node;
}
void BTree::splitChild(Node *parent, int i) {

}
//Helper to check if node holds too many keys
bool BTree::insertOverflow(Node *node, double key, int songIndex) {
    int i = lower_bound(node->keys.begin(), node->keys.end(), key) - node->keys.begin();
    if (i < static_cast<int>(node->keys.size()) && node->keys[i] == key) {
        node->indices[i].push_back(songIndex);
        return false;
    }
    if (node->leaf) {
        node->keys.insert(node->keys.begin() + i, key);
        node->indices.insert(node->indices.begin() + i, vector<int> (1, songIndex));
    }else if (insertOverflow(node->children[i], key, songIndex)) {
        splitChild(node, i);
    }
    return static_cast<int>(node->keys.size()) == 2 * t;
}
//Gives subtrees within the range low-high
void BTree::rangeHelper(const Node *node, double low, double high, vector<int>& result){
    if (node == nullptr) return;
    int n = node->keys.size();
    int i = lower_bound(node->keys.begin(), node->keys.end(), low) - node->keys.begin();
    for (int j = i; j < n; j++) {
        if (!node->leaf) rangeHelper(node->children[j], low, high, result);
        if (node->keys[j] > high) {
            return;
        }
        result.insert(result.end(), node->indices[j].begin(), node->indices[j].end());
    }
    if (!node->leaf) rangeHelper(node->children[n], low, high, result);
}

vector<int> BTree::range(double low, double high) const {

}
size_t BTree::indexBytes(const Node *node) {

}

bool BTree::validNode(const Node *node, bool isRoot, int depth, int &leafDepth, const double *minKey,
    const double *maxKey) const {
    int n = node->keys.size();
    if (n > 2 * t - 1 || (!isRoot && n < t -1) || (isRoot && n < 1)) return false;
    if (node->indices.size() != node->keys.size()) return false;
    for (int i = 0; i < n; i++) {
        if (i > 0 && node->keys[i] <= node->keys[i - 1]) return false;
        if (minKey != nullptr && node->keys[i] <= node->keys[i]) return false;
        if (maxKey != nullptr && node->keys[i] >= node->keys[i]) return false;
        if (node->indices[i].empty()) return false;
    }
    if (node->leaf) {
        if (!node->children.empty()) return false;
        if (leafDepth == -1) leafDepth = depth;
        return leafDepth == depth;
    }
    if (static_cast<int>(node->children.size()) != n + 1) return false;
    for (int i = 0; i <= n; i++) {
        const double* childMin = (i == 0) ? minKey : &node->keys[i - 1];
        const double* childMax = (i == n) ? maxKey : &node->keys[i];
        if (!validNode(node->children[i], false, depth + 1, leafDepth, childMin, childMax)) return false;
    }
    return true;
}

BTree::BTree(int minDegree) {
    if (minDegree < 2) {
        t = 2;
    }else {
        t = minDegree;
    }
}

BTree::~BTree() {
    clear();
}

void BTree::insert(double key, int songIndex) {

}

vector<int> BTree::find(double key) const {

}

void BTree::clear() {
    destroy(root);
    root = nullptr;
    nodes = 0;
}

int BTree::height() const {
    int height = 0;
    const Node* node = root;
    while (node != nullptr) {
        height++;
        node = node->leaf ? nullptr : node->children[0];
    }
    return height;
}

size_t BTree::nodeCount() const {
    return nodes;
}

size_t BTree::memoryBytes() const {

}

string BTree::name() const {
    return "B-Tree (t=" + to_string(t) + ")";
}

bool BTree::isValid() const {
}

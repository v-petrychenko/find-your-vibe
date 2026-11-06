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
    Node* full = parent->children[i];
    Node* right = makeNode(full->leaf);
    //Split right
    for (int j = t; j < 2 * t; j++) {
        right->keys.push_back(full->keys[j]);
        right->indices.push_back(vector<int>());
        right->indices.back().swap(full->indices[j]);
    }
    if (!full->leaf) {
        for (int k = t; k <= 2* t; k++) {
            right->children.push_back(full->children[k]);
        }
        full->children.resize(t);
    }
    //Move key up
    double medianKey = full->keys[t - 1];
    vector<int> medianIndex;
    medianIndex.swap(full->indices[t - 1]);
    full->keys.resize(t-1);
    full->indices.resize(t - 1);
    parent->keys.insert(parent->keys.begin() + i, medianKey);
    parent->indices.insert(parent->indices.begin() + i, vector<int>());
    parent->indices[i].swap(medianIndex);
    parent->children.insert(parent->children.begin() + i + 1, right);

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
    vector<int> result;
    if (low > high) return result;
    rangeHelper(root, low, high, result);
    return result;
}
size_t BTree::indexBytes(const Node *node) {
    if (node == nullptr) return 0;
    size_t bytes = node->keys.capacity() * sizeof(double);
    bytes += node->children.capacity() * sizeof(Node*);
    bytes += node->indices.capacity() * sizeof(vector<int>);
    for (const vector<int>& list : node->indices) {
        bytes += list.capacity() * sizeof(int);
    }
    for (const Node* child : node->children) {
        bytes += indexBytes(child);
    }
    return bytes;
}

bool BTree::validNode(const Node *node, bool isRoot, int depth, int &leafDepth, const double *minKey,
    const double *maxKey) const {
    int n = node->keys.size();
    if (n > 2 * t - 1 || (!isRoot && n < t -1) || (isRoot && n < 1)) return false;
    if (node->indices.size() != node->keys.size()) return false;
    for (int i = 0; i < n; i++) {
        if (i > 0 && node->keys[i] <= node->keys[i - 1]) return false;
        if (minKey != nullptr && node->keys[i] <= *minKey) return false;
        if (maxKey != nullptr && node->keys[i] >= *maxKey) return false;
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
    if (root == nullptr) root = makeNode(true);
    bool overflow = insertOverflow(root, key, songIndex);
    if (overflow) {
        Node* newRoot = makeNode(false);
        newRoot->children.push_back(root);
        root = newRoot;
        splitChild(root, 0);
    }
}

vector<int> BTree::find(double key) const {
    const Node* node = root;
    while (node != nullptr) {
        int i = lower_bound(node->keys.begin(), node->keys.end(), key) -  node->keys.begin();
        if (i < static_cast<int>(node->keys.size()) && node->keys[i] == key) return node->indices[i];
        if (node->leaf) return {};
        node= node->children[i];
    }
    return{};
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
    return nodes * sizeof(Node) + indexBytes(root);
}

string BTree::name() const {
    return "B-Tree (t=" + to_string(t) + ")";
}

bool BTree::isValid() const {
    if (root == nullptr) return true;
    int leafDepth = -1;
    return validNode(root, true, 1, leafDepth, nullptr, nullptr);
}


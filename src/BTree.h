#pragma once
#include <vector>
#include <iostream>
#include "IndexTree.h"
using namespace std;

class BTree : public IndexTree {
    struct Node {
        bool leaf;
        vector<double> keys;
        vector<vector<int>> indices;
        vector<Node*> children;
        Node(bool isLeaf, int t) : leaf(isLeaf) {
            keys.reserve(2 * t - 1);
            indices.reserve(2 * t - 1);
            if (!isLeaf) {
                children.reserve(2 * t);
            }
        }
    };
    int t;
    Node* root = nullptr;
    size_t nodes = 0;
    Node* makeNode(bool leaf);
    static void destroy(Node* node);
    void splitChild(Node* parent, int i);
    bool insertOverflow(Node* node, double key, int songIndex);
    static void rangeHelper(const Node* node, double low, double high, vector<int>& result);
    static size_t indexBytes(const Node* node);
    bool validNode(const Node* node, bool isRoot, int depth, int& leafDepth, const double* minKey, const double* maxKey) const;
public:
    explicit BTree(int minDegree = 32); //Change for different tree depths
    ~BTree() override;
    BTree(const BTree&) = delete;
    BTree& operator=(const BTree&) = delete;
    void insert(double key, int songIndex) override;
    vector<int> find(double key) const override;
    vector<int> range(double low, double high) const override;
    void clear() override;
    int height() const override;
    size_t nodeCount() const override;
    size_t memoryBytes() const override;
    string name() const override;
    bool isValid() const;
};
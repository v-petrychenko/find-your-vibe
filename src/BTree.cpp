#include "BTree.h"

using namespace std;

BTree::Node * BTree::makeNode(bool leaf) {
}

void BTree::destroy(Node *node) {
}

void BTree::split(Node *parent, int i) {
}

void BTree::insertWithRoom(Node *node, double key, int songIndex) {
}

void BTree::rangeHelper(const Node *node, double low, double high, vector<int> &result) {
}

size_t BTree::indexBytes(const Node *node) {
}

bool BTree::validNode(const Node *node, bool isRoot, int depth, int &leafDepth, const double *minKey,
    const double *maxKey) const {
}

BTree::BTree(int minDegree) {
}

BTree::~BTree() {
}

BTree::BTree(const BTree &) {
}

BTree & BTree::operator=(const BTree &) {
}

void BTree::insert(double key, int songIndex) {
}

vector<int> BTree::find(double key) const {
}

vector<int> BTree::range(double low, double high) const {
}

void BTree::clear() {
}

int BTree::height() const {
}

size_t BTree::nodeCount() const {
}

size_t BTree::memoryBytes() const {
}

string BTree::name() const {
}

bool BTree::isValid() const {
}

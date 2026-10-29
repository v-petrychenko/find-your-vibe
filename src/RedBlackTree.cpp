
#include "RedBlackTree.h"
#include <iostream>

using namespace std;

RedBlackTree::RedBlackTree() = default;

RedBlackTree::~RedBlackTree() {
    clear();
}


void RedBlackTree::insert(double key, int songIndex) {
    Node* current = root;
    Node* parent = nullptr;

    //cout << "inserted: " << key << endl;

    //Find where to insert node
    while (current != nullptr) {
        parent = current;

        if (key == current->key) {
            current->indices.push_back(songIndex);
            //cout << "duplicate: " << key << endl;
            return;
        }

        if (key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    Node* newNode = new Node(key, songIndex, parent);

    //Connect node to the parent
    if (parent == nullptr) {
        root = newNode;
    }
    else if (key < parent-> key) {
        parent->left = newNode;
    }
    else {
        parent->right = newNode;
    }

    count++;
    root->color = Color::Black;

    //cout << "Total: " << count << endl;
}



vector<int> RedBlackTree::find(double key) const {
    Node* current = root;
    //Search for key
    while (current != nullptr) {
        if (key == current-> key) {
            //cout << "key " << key << endl;
            return current->indices;
        }
        if (key < current-> key) {
            current =  current->left;
        }
        else {
            current = current->right;
        }
        
    }
    return {};
}


vector<int> RedBlackTree::range(double lo, double hi) const {
    return {};
}

void RedBlackTree::clear() {
    destroy(root);
    root = nullptr;
    count = 0;
}

void RedBlackTree::destroy(Node* node) {
    if (node == nullptr)  {
        return;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
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

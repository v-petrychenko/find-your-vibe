
#include "RedBlackTree.h"
#include <iostream>
#include <algorithm>

using namespace std;

RedBlackTree::RedBlackTree() = default;

RedBlackTree::~RedBlackTree() {
    clear();
}


void RedBlackTree::insert(double key, int songIndex) {
    Node* current = root;
    Node* parent = nullptr;


    //Find where to insert node
    while (current != nullptr) {
        parent = current;

        if (key == current->key) {
            current->indices.push_back(songIndex);
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
    fixInsert(newNode);
}



vector<int> RedBlackTree::find(double key) const {
    Node* current = root;
    //Search for key
    while (current != nullptr) {
        if (key == current-> key) {
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
    vector<int> result;
    if (lo > hi) {
        return result;
    }
    rangeHelper(root, lo, hi, result);
    return result;
}

void RedBlackTree::rangeHelper(const Node* node, double lo, double hi, vector<int>& result) {
    if (node == nullptr) {
        return;
    }

    //only search left (if val in range)
    if (node->key > lo) {
         rangeHelper(node->left, lo, hi, result);
    }

    if (node->key >= lo && node-> key <= hi) {
        for (int index : node->indices) {
            result.push_back(index) ;
        }
    }

    //Only search right (if val in range)
    if (node->key < hi) {
        rangeHelper(node-> right, lo, hi, result);
    }
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
    return heightOf(root);
}

int RedBlackTree::heightOf(const Node* node) {
    if (node == nullptr) {
        return 0;
    }
    //Find height of L and R subtrees
    int leftHeight = heightOf(node-> left);
    int rightHeight = heightOf(node->right);

    return 1 + max(leftHeight, rightHeight);
}


size_t RedBlackTree::nodeCount() const {
    return count;
}


size_t RedBlackTree::memoryBytes() const {
    return count * sizeof(Node) + indexBytes(root);
}

size_t RedBlackTree::indexBytes(const Node* node) {
    if (node == nullptr) {
        return 0;
    }
    //Count memory used by songs
    // memory(bytes) = count * size(node) + bytes(root)
    size_t bytes = node-> indices.capacity() * sizeof(int);

    bytes +=  indexBytes(node->left);
    bytes += indexBytes(node->right);
    return bytes;
}


string RedBlackTree::name() const {
    return "Red-Black Tree";
}

void RedBlackTree::rotateLeft(Node* node) {
    Node* rightChild = node ->right;

    //Move left subtree of right child
    node->right = rightChild->left;
    if (rightChild->left != nullptr) {
        rightChild->left->parent = node;
    }

    //Connect right child to old parent
    rightChild->parent = node-> parent;

    if (node->parent == nullptr) {
        root = rightChild;
    }
    else if (node == node->parent->left) {
        node->parent->left  = rightChild;
    }
    else {
        node->parent->right = rightChild;
    }

    //Move node under right child
    rightChild->left = node;
    node->parent = rightChild;
}

void RedBlackTree::rotateRight(Node* node) {
    Node* leftChild =  node->left;

    //Move right subtree of left child
    node-> left = leftChild->right;
    if (leftChild->right != nullptr) {
        leftChild->right->parent = node;
    }

    //Connect left child to old parent
    leftChild->parent = node->parent;

    if (node->parent == nullptr) {
        root = leftChild;
    }
    else if (node == node->parent->right) {
        node->parent->right = leftChild;
    }
    else {
        node->parent->left = leftChild;
    }

    //Move node under left child
    leftChild->right = node;
    node->parent = leftChild;
}

bool RedBlackTree::isRed(const Node* node) {
    return node != nullptr && node->color == Color::Red;
}


void RedBlackTree::fixInsert(Node* node) {
    while (node != root && isRed(node->parent)) {
        Node* parent = node-> parent;
        Node* grandparent = parent->parent;

        if (parent == grandparent->left) {
            Node* uncle =  grandparent->right;

            //Uncle is red
            if (isRed(uncle)) {
                //cout << "Recolor: " << grandparent->key << "\n";
                parent->color = Color::Black;
                uncle->color = Color::Black;
                grandparent->color = Color::Red;
                node = grandparent;
            }
            else {
                //Node is on inside
                if (node == parent-> right) {
                    rotateLeft(parent);
                    node = parent;
                }

                //Rotate grandparent
                parent->color = Color::Black;
                grandparent ->color = Color::Red ;
                rotateRight(grandparent);
            }
        }
        else {
            Node* uncle = grandparent->left;

            //Uncle is red
            if (isRed(uncle)) {
                //cout << "Recolor: " << grandparent->key << "\n";
                parent->color = Color::Black;
                uncle->color = Color::Black;
                grandparent->color = Color::Red;
                node = grandparent;
            }
            else {
                //Node is on inside
                if (node  == parent->left) {
                    rotateRight(parent);
                    node = parent;
                }

                //Rotate grandparent
                parent->color = Color::Black;
                grandparent-> color = Color::Red;
                rotateLeft (grandparent);
            }
        }
    }

    root->color = Color::Black;
}

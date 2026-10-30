
#include "RedBlackTree.h"
#include <iostream>

using namespace std;

void check(bool result, string name) {
    cout << name << ": " << (result ? "PASS" : "FAIL") << "\n";
}

void testRotations() {
    //left rotation
    RedBlackTree leftTree;
    leftTree.insert(10, 0);
    leftTree.insert(20, 1);
    leftTree.insert(30, 2);

    leftTree.rotateLeft(leftTree.root);
    check(leftTree.root->key == 20, "Left rotation");
    check(leftTree.height() == 2, "Left rotation height");

    //right rotation
    RedBlackTree rightTree;
    rightTree.insert(30, 0);
    rightTree.insert(20, 1);
    rightTree.insert(10, 2);

    rightTree.rotateRight(rightTree.root);
    check(rightTree.root->key == 20, "Right rotation");
    check(rightTree.height() == 2, "Right rotation height");
}

int main() {
    RedBlackTree tree;

    tree.insert(50, 0);
    tree.insert(30, 1);
    tree.insert(70, 2);
    tree.insert(30, 3);

    //insert
    check(tree.nodeCount() == 3, "Insert");

    //find
    vector<int> result = tree.find(30);
    check(result == vector<int>{1, 3}, "Find");

    //range
    vector<int> rangeResult = tree.range(25, 65);
    check(rangeResult == vector<int>{1, 3, 0}, "Range");

    //height
    check(tree.height() == 2, "Height");

    //memory usage
    size_t before = tree.memoryBytes();
    tree.insert(40, 4);
    check(tree.memoryBytes() > before, "Memory");

    //range boundaries
    check(tree.range(30, 50) == vector<int>{1, 3, 4, 0}, "Range boundaries");

    //single value range
    check(tree.range(70, 70) == vector<int>{2}, "Single value range");

    //invalid range
    check(tree.range(70, 30).empty(), "Invalid range");

    //missing key
    check(tree.find(999).empty(), "Missing key");

    //duplicate insertion
    tree.insert(30, 5);
    check(tree.nodeCount() == 4, "Duplicate count");
    check(tree.find(30) == vector<int>{1, 3, 5}, "Duplicate values");

    //clear
    tree.clear();
    check(tree.nodeCount() == 0, "Clear count");
    check(tree.height() == 0, "Clear height");
    check(tree.find(30).empty(), "Clear find");

    //rotations
    testRotations();
    return 0;
}

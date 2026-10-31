#include "RedBlackTree.h"
#include <iostream>

using namespace std;

bool testsPassed = true;

void check(bool result, string name) {
    cout << name << ": " << (result ? "PASS" : "FAIL") << "\n";
    if (!result) {
        testsPassed = false;
    }
}

void testBalancing() {
    //left rotation
    RedBlackTree tree1;
    tree1.insert(10, 0);
    tree1.insert(20, 1);
    tree1.insert(30, 2);
    check(tree1.height() == 2, "Left rotation");

    //right rotation
    RedBlackTree tree2;
    tree2.insert(30, 0);
    tree2.insert(20, 1);
    tree2.insert(10, 2);
    check(tree2.height() == 2, "Right rotation");

    //left-right rotation
    RedBlackTree tree3;
    tree3.insert(30, 0);
    tree3.insert(10, 1);
    tree3.insert(20, 2);
    check(tree3.height() == 2, "Left-right rotation");

    //right-left rotation
    RedBlackTree tree4;
    tree4.insert(10, 0);
    tree4.insert(30, 1);
    tree4.insert(20, 2);
    check(tree4.height() == 2, "Right-left rotation");

    //recoloring
    RedBlackTree tree5;
    tree5.insert(10, 0);
    tree5.insert(5, 1);
    tree5.insert(15, 2);
    tree5.insert(1, 3);
    check(tree5.height() == 3, "Recoloring");

    check(tree1.isValid() && tree2.isValid() && tree3.isValid() && tree4.isValid() && tree5.isValid(), "Balancing valid");
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
    
    check(tree.isValid(), "Clear valid");

    //balancing
    testBalancing();
    
    return testsPassed ? 0 : 1;
}

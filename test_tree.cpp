
#include "RedBlackTree.h"
#include <iostream>

using namespace std;

void check(bool result, string name) {
    cout << name << ": " << (result ? "PASS" : "FAIL") << "\n";
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

    return 0;
}

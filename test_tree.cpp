
#include "RedBlackTree.h"
#include <iostream>

using namespace std;

int main() {
    RedBlackTree tree;

    //#1 insert nodes
    tree.insert(50, 0);
    tree.insert(30, 1);
    tree.insert(70, 2);
    tree.insert(30, 3);

    if (tree.nodeCount() == 3) {
        cout << "#1 PASS" << endl;
    }
    else {
        cout << "#1 FAIL" << endl;
    }

    //#2 find key
    vector<int> result = tree.find(30);
    if (result == vector<int>{1, 3}) {
        cout << "#2 PASS" << endl;
    }
    else {
        cout << "#2 FAIL" << endl;
    }

    return 0;
}

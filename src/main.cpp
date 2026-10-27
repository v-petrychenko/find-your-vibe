#include <iostream>
#include <string>
#include <vector>

#include "CsvLoader.h"
#include "RedBlackTree.h"
#include "Song.h"

static void printMenu() {
}

int main(int argc, char* argv[]) {
    std::string dataPath = argc > 1 ? argv[1] : "data/dataset.csv";
    std::vector<Song> songs;
    RedBlackTree rbTree;

    printMenu();
    return 0;
}

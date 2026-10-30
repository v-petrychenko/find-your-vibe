
#include <iostream>
#include <string>
#include <vector>

#include "CsvLoader.h"
#include "RedBlackTree.h"
#include "Song.h"

using namespace std;

static void printMenu() {
}

int main(int argc, char* argv[]) {
    string dataPath = argc > 1 ? argv[1] : "data/dataset.csv";
    vector<Song> songs;
    RedBlackTree rbTree;

    //Load songs
    try {
        songs = loadSongs(dataPath);
    }
    catch (const exception& e) {
        cout << e.what() << endl;
        return 1;
    }

    //Insert songs into the BST
    for (int i = 0; i < songs.size(); i++) {
        double key = featureValue(songs[i], Feature::Tempo);
        rbTree.insert(key, i);
    }

    cout << "Total songs: " << songs.size() << endl;
    cout << "Nodes: " << rbTree.nodeCount() << endl;
    cout << "Height: " << rbTree.height() << endl;
    cout << "Memory: " << rbTree.memoryBytes() << " bytes" << endl;

    //test finding tempo
    vector<int> found = rbTree.find(120.0);
    cout << "Songs with tempo 120: " << found.size() << endl;

    //Print first few songs
    for (int i = 0; i < found.size() && i < 3; i++) {
        cout << songs[found[i]].trackName << endl;
    }

    //test range search
    vector<int> results = rbTree.range(160, 180);
    cout << "Songs with tempo 160-180: " << results.size() << endl;

    printMenu();
    return 0;
}

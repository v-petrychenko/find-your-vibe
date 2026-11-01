
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "CsvLoader.h"
#include "RedBlackTree.h"
#include "IndexTree.h"
#include "Song.h"

using namespace std;

static string featureName(Feature feature) {
    switch (feature) {
        case Feature::Tempo: return "Tempo";
        case Feature::Energy: return "Energy";
        case Feature::Danceability: return "Danceability";
        case Feature::Valence: return "Valence";
        case Feature::Popularity: return "Popularity";
    }
    return "";
}

static void printMenu() {
    cout << "\n===== FIND YOUR VIBE =====\n";
    cout << "1. Search by range\n";
    cout << "2. Search exact value\n";
    cout << "3. Change feature\n";
    cout << "4. Tree statistics\n";
    cout << "5. Verify tree results\n";
    cout << "6. Run benchmark\n";
    cout << "0. Exit\n";
}

//Read menu choice
static int readChoice(int max) {
    string line;

    while (true) {
        cout << "Enter choice: ";
        if (!getline(cin, line)) {
            return -1;
        }
        stringstream input(line);
        int choice;
        char extra;

        if (input>>choice && !(input>>extra) && choice >= 0 && choice <= max) {
            return choice;
        }
        cout << "Invalid choice.\n";
    }
}

static void buildTrees(const vector<Song>& songs, const vector<IndexTree*>& trees, Feature feature) {
    for (IndexTree* tree : trees) {
        tree->clear();

        for (size_t i = 0; i < songs.size(); i++) {
            tree-> insert(featureValue(songs[i], feature), i);
        }
    }
}

//Print tree stats
static void printStats(const vector<IndexTree*>& trees) {
    for (IndexTree* tree : trees) {
        cout << "\n" << tree->name() << endl;
        cout << "Nodes: " << tree->nodeCount() << endl;
        cout << "Height: " << tree-> height() << endl;
        cout << "Memory: " << tree->memoryBytes() << " bytes" << endl;
    }
}

int main(int argc, char* argv[]) {
    string dataPath = argc > 1 ? argv[1] : "data/dataset.csv";
    vector<Song> songs;
    RedBlackTree rbTree;
    vector<IndexTree*> trees = {&rbTree};
    Feature feature = Feature::Tempo;

    //Load songs
    try {
        songs = loadSongs(dataPath);
    }
    catch (const exception& e) {
        cout << e.what() << endl;
        return 1;
    }
    if (songs.empty()) {
        cout << "No songs loaded.\n";
        return 1;
    }

    //Build trees
    buildTrees(songs, trees, feature);

    cout << "Total songs: " << songs.size() << endl;
    cout << "Current feature: " << featureName(feature) << endl;

    //Main menu
    while (true) {
        printMenu();
        int choice = readChoice(6);
        if (choice == 0 || choice == -1) {
            break;
        }
        switch (choice) {
            case 3: {
                cout << "\nSelect feature:\n";
                cout << "1. Tempo\n";
                cout << "2. Energy\n";
                cout << "3. Danceability\n";
                cout << "4. Valence\n";
                cout << "5. Popularity\n";
                cout << "0. Back\n";

                int selected = readChoice(5);
                if (selected == -1) {
                    return 0;
                } else if (selected == 0) {
                    break;
                }

                Feature features[] = {
                    Feature::Tempo,
                    Feature::Energy,
                    Feature::Danceability,
                    Feature::Valence,
                    Feature::Popularity
                };

                Feature newFeature = features[selected - 1];
                if (newFeature != feature) {
                    feature = newFeature;
                    buildTrees(songs, trees, feature);
                    cout << "Trees rebuilt for " << featureName(feature) << endl;
                }
                else {
                    cout << "Feature already selected.\n";
                }
                break;
            }

            case 4:
                cout << "\nCurrent feature: " << featureName(feature) << endl;
                printStats(trees);
                break;

            default:
                cout << "Not implemented yet.\n";
                break;
        }
    }
    cout << "Goodbye!\n";
    return 0;
}

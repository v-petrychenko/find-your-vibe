
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <cctype>

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

//Read search value
static bool readNumber(string prompt, double& value) {
    string line;
    while (true) {
        cout << prompt;
        if (!getline(cin, line)) {
            return false;
        }

        stringstream input(line);
        char extra;

        if (input>>value && !(input>>extra)) {
            return true;
        }
        cout << "Invalid number.\n";
    }
}

//Convert text to lowercase
static string lowerCase(string text) {
    for (size_t i = 0; i < text.size(); i++) {
        text[i] = tolower((unsigned char)text[i]);
    }
    return text;
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

//Print matching songs
static bool printSongs(const vector<Song>& songs, const vector<int>& results) {
    if (results.empty()) {
        cout << "No songs found.\n";
        return true;
    }

    //Genre filter
    string genre;
    cout << "Genre (Enter for all): ";
    if (!getline(cin, genre)) {
        return false;
    }
    genre = lowerCase(genre);

    //Sort by popularity
    vector<int> sorted = results;
    sort(sorted.begin(), sorted.end(),[&songs](int a, int b) {
        if (songs[a].popularity == songs[b].popularity) {
            return a < b;
        }
        return songs[a].popularity > songs[b].popularity;
    });

    //Remove duplicates
    vector<int> shown;

    for (int index : sorted) {
        const Song& song = songs[index];
        if (!genre.empty() && lowerCase(song.genre) !=genre) {
            continue;
        }
        bool duplicate = false;
        for (int oldIndex : shown) {
            if (song.trackName == songs[oldIndex].trackName &&
                song.artists == songs[oldIndex].artists){
                duplicate  = true;
                break;
            }
        }
        if (duplicate) {
            continue;
        }
        shown.push_back(index);
        if (shown.size() == 10) {
            break;
        }
    }

    if (shown.empty()) {
        cout << "No songs found.\n";
        return true;
    }
    cout << "\nTop songs:\n";
    for (size_t i = 0; i < shown.size(); i++) {
        const Song& song =  songs[shown[i]];
        cout << i + 1 << ". " << song.trackName << " - "
             << song.artists << " (" << song.popularity << ")\n";
    }

    return true;
}

//Search for range
static bool searchRange(const vector<Song>& songs, const vector<IndexTree*>& trees, Feature feature) {
    double lo, hi;
    cout << "\nRange search for " << featureName(feature) << endl;
    if (!readNumber("Minimum: ",lo)) {
        return false;
    }
    if (!readNumber("Maximum: ", hi)) {
        return false;
    }

    if (lo > hi ) {
        swap(lo, hi);
    }

    vector<int> results;

    for (IndexTree* tree :trees) {
        auto start = chrono::steady_clock::now();
        vector<int> found = tree->range(lo, hi);
        auto end = chrono::steady_clock::now();
        double time = chrono::duration<double, micro>(end - start).count();
        cout << tree->name() << ": " << found.size() << " matches, " << time << " us\n";
        if (tree == trees[0]) {
            results  = found;
        }
    }

    return printSongs(songs, results);
}

//Search for exact
static bool searchExact(const vector<Song>& songs, const vector<IndexTree*>& trees, Feature feature) {
    double key;
    cout << "\nExact search for " << featureName(feature) << endl;

    if (!readNumber("Value: ", key)) {
        return false;
    }
    vector<int> results;

    for (IndexTree* tree : trees) {
        auto start = chrono::steady_clock::now();
        vector<int> found = tree->find(key);
        auto end = chrono::steady_clock::now();
        double time = chrono::duration<double, micro>(end - start).count();
        cout << tree-> name() << ": " << found.size() << " matches, " << time << " us\n";
        if (tree ==trees[0]) {
            results = found;
        }
    }

    return printSongs(songs,results);
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
            case 1:
                if (!searchRange(songs, trees, feature)) {
                    return 0;
                }
                break;

            case 2:
                if (!searchExact(songs, trees, feature)) {
                    return 0;
                }
                break;
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

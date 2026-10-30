
#include "CsvLoader.h"
#include <iostream>

using namespace std;

void check(bool result, string name) {
    cout << name << ": " << (result ? "PASS" : "FAIL") << "\n";
}

int main() {
    //CVS Slit
    auto a = splitCsvLine("1,Hello,World");
    check(a.size() == 3, "Basic CSV");

    auto b = splitCsvLine("1,\"Hello, World\",Test");
    check(b.size() == 3 && b[1] == "Hello, World", "Quoted comma");

    auto c = splitCsvLine("1,Hello,");
    check(c.size() == 3 && c[2] == "", "Empty field");

    auto d = splitCsvLine("1,\"Hello \"\"World\"\"\",Test");
    check(d.size() == 3 && d[1] == "Hello \"World\"", "Escaped quotes");

    //Load dataset
    try {
        vector<Song> songs = loadSongs("data/dataset.csv");

        check(songs.size() == 114000, "Song count");

        if (songs.size() == 114000) {
            check(songs[0].trackName == "Comedy", "First song");
            check(songs[19].albumName == "Cover Sessions, Vol. 4", "Album comma");
            check(songs[44].trackName ==
                  "Speak Your Mind (From the Netflix Series \"We The People\")",
                  "Track quotes");

            check(featureValue(songs[0], Feature::Tempo) == songs[0].tempo,
                  "Feature value");
        }
    }
    catch (const exception& e) {
        cout << e.what() << "\n";
    }
    return 0;
}

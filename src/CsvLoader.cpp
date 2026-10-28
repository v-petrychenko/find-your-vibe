#include "CsvLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace {
const int COL_TRACK_ID = 1;
const int COL_ARTISTS = 2;
const int COL_ALBUM = 3;
const int COL_TRACK_NAME = 4;
const int COL_POPULARITY = 5;
const int COL_DURATION = 6;
const int COL_EXPLICIT = 7;
const int COL_DANCEABILITY = 8;
const int COL_ENERGY = 9;
const int COL_VALENCE = 17;
const int COL_TEMPO = 18;
const int COL_GENRE = 20;
const int COLUMN_COUNT = 21;
}

//Return the value of a feature
double featureValue(const Song& song, Feature feature) {
    switch (feature) {
        case Feature::Tempo: return song.tempo;
        case Feature::Energy: return song.energy;
        case Feature::Danceability: return song.danceability;
        case Feature::Valence: return song.valence;
        case Feature::Popularity: return song.popularity;
    }
    return 0.0;
}


std::vector<Song> loadSongs(const std::string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        throw runtime_error("Could not open " + path);
    }

    vector<Song> songs;
    string line;
    int skipped = 0;

    while (getline(file, line)) {
        vector<string> fields = splitCsvLine(line);

        if (fields.size() != COLUMN_COUNT) { //Check that row has 21 columns
            skipped++;
            continue;
        }

        Song song;

        try { //assign fields
            song.trackId = fields[COL_TRACK_ID];
            song.artists = fields[COL_ARTISTS];
            song.albumName = fields[COL_ALBUM];
            song.trackName = fields[COL_TRACK_NAME];
            song.genre = fields[COL_GENRE];
            song.popularity = stoi(fields[COL_POPULARITY]);
            song.durationMs = stoi(fields[COL_DURATION]);
            song.isExplicit = fields[COL_EXPLICIT] == "True";
            song.danceability = stod(fields[COL_DANCEABILITY]);
            song.energy = stod(fields[COL_ENERGY]);
            song.valence = stod(fields[COL_VALENCE]);
            song.tempo = stod(fields[COL_TEMPO]);
        }
        catch (const exception&) {
            skipped++;
            continue;
        }

    }
    cout << "Loaded " << songs.size() << " songs, skipped " << skipped << " rows\n";
    return songs;
}

//Split file using commas
vector<string> splitCsvLine(const string& line) { 
    vector<string> fields;
    stringstream ss(line);
    string field;

    while (getline(ss, field, ',')) {
        fields.push_back(field);
    }

    return fields;
}

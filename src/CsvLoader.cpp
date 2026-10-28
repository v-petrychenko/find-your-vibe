#include "CsvLoader.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

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
    return {};
}

std::vector<std::string> splitCsvLine(const std::string& line) {
    return {};
}

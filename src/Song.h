#pragma once

#include <string>

//One row of the dataset
struct Song {
    std::string trackId;
    std::string trackName;
    std::string artists;
    std::string albumName;
    std::string genre;
    int popularity = 0;
    int durationMs = 0;
    bool isExplicit = false;
    double danceability = 0.0;
    double energy = 0.0;
    double valence = 0.0;
    double tempo = 0.0;
};

//Audio features the trees can be found with
enum class Feature {
    Tempo,
    Energy,
    Danceability,
    Valence,
    Popularity
};

//Value of the feature for a song (tree key)
double featureValue(const Song& song, Feature feature);

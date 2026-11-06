#include "Benchmark.h"
#include "BTree.h"
#include "IndexTree.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

#include "RedBlackTree.h"
using namespace std;

namespace {
    const int BUILD_RUNS = 3;
    const int EXACT_LOOKUPS = 2000;
    const int RANGE = 200;
    const double WIDTHS[] = {0.01, 0.05, 0.25};
    const int WIDTH_COUNT = 3;
    const char* SAVE_PATH = "benchmark_results.csv";
}
struct lookUp {
    double low, high;
};
struct Result {
    string name;
    double buildUs = 0;
    double exactUs = 0;
    double rangeUs[WIDTH_COUNT] = {0,0,0};
    int height;
    size_t nodes = 0;
};
string searchTitle(Feature feature) {
    switch (feature) {
        case Feature::Tempo: return "Tempo";
        case Feature::Energy: return "Energy";
        case Feature::Danceability: return "Danceability";
        case Feature::Valence: return "Valence";
        case Feature::Popularity: return "Popularity";
    }
    return "";
}
string align(const string& text, size_t width, bool alignLeft) {
    if (text.size() >= width) return text;
    string spaces(width - text.size(), ' ');
    if (alignLeft) {
        return text + spaces;
    }else{
        return spaces + text;
    }
}
string truncate(double value) {
    if (value >= 10000) return to_string(static_cast<long long>(value));
    double truncated = static_cast<long long>(value * 100) / 100.0;
    ostringstream out;
    out<<truncated;
    return out.str();
}
double timerUs(chrono::steady_clock::time_point start) {
    return chrono::duration<double, micro>(chrono::steady_clock::now() - start).count();
}
void runBenchmark(const vector<Song> &songs, Feature feature, bool writeCsv) {
    
}


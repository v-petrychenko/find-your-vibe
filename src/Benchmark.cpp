#include "Benchmark.h"
#include "BTree.h"
#include "IndexTree.h"
#include "IndexTree.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
using namespace std;

namespace {
    const int BUILD_RUNS = 3;
    const int EXACT_LOOKUPS = 2000;
    const int RANGE = 200;
    const double WIDTHS[] = {0.01, 0.05, 0.25};
    const int WIDTHS_COUNT = 3;
    const char* SAVE_PATH = "benchmark_results.csv";
}
struct Query {

};
struct Result {

};
string searchTitle(Feature feature) {

}
string align(const string& text, size_t width, bool leftAlign) {

}
string truncate(double value) {

}
double timerUs(chrono::steady_clock::time_point start) {

}
void runBenchmark(const vector<Song> &songs, Feature feature, bool writeCsv = true) {

}

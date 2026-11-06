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
    if (songs.empty()) {
        cout<<"No songs to benchmark."<<endl;
        return;
    }
    RedBlackTree rb;
    BTree b8(8), b32(32), b64(64);
    vector<IndexTree*> trees = {&rb, &b8, &b32, &b64};
    mt19937 rand(42);
    uniform_int_distribution<size_t> pick(0, songs.size() - 1);
    double minVal = featureValue(songs[0],feature);
    double maxVal = minVal;
    for (const Song& song : songs) {
        double v = featureValue(song,feature);
        minVal = min(minVal, v);
        maxVal = max(maxVal, v);
    }
    vector<double> exactKeys;
    for (int i = 0; i < EXACT_LOOKUPS; i++) {
        exactKeys.push_back(featureValue(songs[pick(rand)], feature));
    }
    vector<vector<lookUp>> ranges(WIDTH_COUNT);
    for (int j = 0; j < WIDTH_COUNT; j++) {
        double width = (maxVal - minVal) * WIDTHS[j];
        uniform_real_distribution<double> startDist(minVal, maxVal - width);
        for (int k = 0; k < RANGE; k++) {
            double low = startDist(rand);
            ranges[j].push_back(lookUp{low, low + width});
        }
    }
    cout<<"\nBenchmarking "<<searchTitle(feature)<<" on "<< songs.size()<<" songs..."<<endl;
    vector<Result> results;
    size_t checksum = 0;
    size_t totalMatches[WIDTH_COUNT] = {0, 0, 0};
    //Tree build time
    for (size_t i = 0; i < trees.size(); i++) {
        IndexTree* tree = trees[i];
        Result r;
        r.name = tree->name();
        double insertTime = 0.0;
        for (int j = 0; j < BUILD_RUNS; j++) {
            tree->clear();
            auto start = chrono::steady_clock::now();
            for (size_t s = 0; s < songs.size(); s++) {
                tree->insert(featureValue(songs[s], feature), static_cast<int>(s));
            }
            insertTime += timerUs(start);
        }
        r.buildUs = (insertTime / BUILD_RUNS);
        r.height = tree->height();
        r.nodes = tree->nodeCount();
        //Exact search times
        auto start = chrono::steady_clock::now();
        for (double key : exactKeys) checksum += tree->find(key).size();
        r.exactUs = timerUs(start) / exactKeys.size();
        //Avg search time
        for (int w = 0; w < WIDTH_COUNT; w++) {
            start = chrono::steady_clock::now();
            for (auto& queue : ranges[w]) {
                size_t found = tree->range(queue.low, queue.high).size();
                checksum += found;
                if (i == 0) totalMatches[w] += found;
            }
            r.rangeUs[w] = timerUs(start) / ranges[w].size();
        }
        results.push_back(r);
        cout<<" completed "<<r.name<<" benchmark"<<endl;
    }
    //Avg matches per range
    double avgMatches[WIDTH_COUNT];
    for (int w = 0; w < WIDTH_COUNT; w++) {
        avgMatches[w] = static_cast<double>(totalMatches[w]) / RANGE;
    }
    //Printing in format
    const size_t labelWidth = 26;
    const size_t colWidth = 17;
    string line(labelWidth + colWidth * results.size(), '-');
    cout<<"\n"<<string(labelWidth + colWidth * results.size(), '=')<<endl;
    cout<<"BENCHMARK: "<<searchTitle(feature)<<" ("<<to_string(songs.size())<<" songs)"<<endl;
    cout<<string(labelWidth + colWidth * results.size(), '=')<<endl;
    cout<<align("", labelWidth, true);
    for (const Result& r : results) {
        cout<<align(r.name, colWidth, false);
    }
    cout<<"\n"<<line<<endl;
    auto printRow = [&](const string& label, const vector<string>& values) {
        cout<<align(label, labelWidth, true);
        for (const string& value : values) {
            cout<<align(value, colWidth, false);
        }
        cout<<endl;
    };
    vector<string> values;
    values.clear();
    for (const Result& r : results) values.push_back(truncate(r.buildUs));
    printRow("Build time (us)", values);
    values.clear();
    for (const Result& r : results) values.push_back(truncate(r.exactUs));
    printRow("Exact search (us/search)", values);
    for (int w = 0; w < WIDTH_COUNT; w++) {
        values.clear();
        for (const Result& r : results) values.push_back(truncate(r.rangeUs[w]));
        printRow("Range " + to_string(static_cast<int>(WIDTHS[w] * 100)) + "% (us/search)", values);
    }
    cout<<line<<endl;
    values.clear();
    for (const Result& r : results) values.push_back(to_string(r.height));
    printRow("Height", values);
    values.clear();
    for (const Result& r : results) values.push_back(to_string(r.nodes));
    printRow("Nodes", values);
    cout<<line<<endl;
    cout<<"\nAverage matches per range search: ";
    for (int w = 0; w < WIDTH_COUNT; w++) {
        cout<<static_cast<int>(WIDTHS[w] * 100)<<"% -> "<<truncate(avgMatches[w])<<(w + 1 < WIDTH_COUNT ? ", " : "\n");
    }
    cout<<"("<<RANGE<<" searches per range width)"<<endl;
    //Write to CSV to stockpile data for different search types
    if (writeCsv) {
        ofstream csv(SAVE_PATH, ios::app | ios::ate);
        if (!csv.is_open()) {
            cout<<"Couldn't write into "<<SAVE_PATH<<endl;
            return;
        }
        if (csv.tellp() == 0) {
            csv<<"feature,tree,build_us,exact_us";
            for (int w = 0; w < WIDTH_COUNT; w++) {
                csv<<",range_"<<static_cast<int>(WIDTHS[w] * 100)<<"%_us";
            }
            csv<<",height,nodes"<<endl;
        }
        for (auto& r : results) {
            csv<<searchTitle(feature)<<",\""<<r.name<<"\","<<r.buildUs<<","<<r.exactUs;
            for (int w = 0; w < WIDTH_COUNT; w++) {
                csv<<","<<r.rangeUs[w];
            }
            csv<<","<<r.height<<","<<r.nodes<<endl;
        }
        cout<<"Results added to "<<SAVE_PATH<<endl;
    }
}


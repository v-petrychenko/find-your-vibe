#pragma once

#include <string>
#include <vector>

#include "Song.h"

//Loads every row from the dataset
std::vector<Song> loadSongs(const std::string& path);

//Splits one CSV line into fields
std::vector<std::string> splitCsvLine(const std::string& line);

Execution steps
-Set up the dataset by putting the dataset.csv in a folder
next to the program so the path is data/dataset.csv
-Build using the CMake file
-The program will load the songs, build a BTree and rbTree (defaulty sorted by tempo) and display the menu
all you need to do is input the number of your selected choice and press enter
-Option 1 will allow you to search in a range of the given feature
-Option 2 lets you search for songs with a specific value corresponding with the selected feature
-Option 3 allows you to pick Tempo, Energy, Danceability, Valence, or Popularity
-Option 4 Lets you see the tree statistics Ex:nodes, height, etc.
-Option 5 Lets you verify the trees against the full song list
-Option 6 Runs a benchmarkl that gives the times if the four total tree in a table
-Option 7 (0) Exits the program
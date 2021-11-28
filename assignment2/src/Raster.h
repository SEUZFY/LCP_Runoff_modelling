#pragma once
#include <vector>

// Storage and access of a raster of a given size

struct Raster {
    std::vector<int> pixels; // where everything is stored
    int nrows, ncols; // number of rows and cols

    // Initialise a raster with rows and cols
    Raster(const int& rows, const int& cols);

    // Fill values of an entire row
    void add_scanline(const int* line);

    // Fill entire raster with zeros
    void fill();

    // Access the value of a raster cell to read or write it
    int& operator()(const int& row, const int& col);

    // Access the value of a raster cell to read it
    int operator()(const int& row, const int& col) const;

    //Access the value of a raster cell to set its value
    void set_value(const int& row, const int& col, const int& value);

};
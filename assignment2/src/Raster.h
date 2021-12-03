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

    // Fill the elevations of the entire raster with zeros
    void fill();

    // Access the value of a raster cell to read or write it
    int& operator()(const int& row, const int& col);

    // Access the value of a raster cell to read it
    int operator()(const int& row, const int& col) const;

    //Access the value of a raster cell to set its value
    void set_value(const int& row, const int& col, const int& value);

};


// A structure that links to a single cell in a Raster


struct RasterCell {
    int row, col; // row and column of the cell
    int elevation;
    int insertion_order;
    bool visited; // if not yet visited: false, once visited: true
    bool listed; // if not yet been added to the list: false, once added: true
    int direct; // direction of current cell, initialized with 0

    // Defines a new link to a cell
    RasterCell(const int& c_row, const int& c_col, const int& elevation, const int& insertion);

    // Define the order of the linked cells (to be used in a priority_queue)
    bool operator<(const RasterCell& other) const;
};

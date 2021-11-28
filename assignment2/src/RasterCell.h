#pragma once

// A structure that links to a single cell in a Raster

struct RasterCell {
    int row, col; // row and column of the cell
    int elevation;
    int insertion_order;

    // Defines a new link to a cell
    RasterCell(int c_row, int c_col, int elevation, int insertion_order);

    // Define the order of the linked cells (to be used in a priority_queue)
    bool operator<(const RasterCell& other) const;
};
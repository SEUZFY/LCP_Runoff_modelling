#include "Raster.h"
#include <cassert>


// Raster


Raster::Raster(const int& rows, const int& cols)
    : nrows(rows), ncols(cols) {
    unsigned int total_pixels = rows * cols; //unsigned int for big size
    pixels.reserve(total_pixels);
}

void Raster::add_scanline(const int* line) {
    for (int i = 0; i < ncols; ++i) pixels.push_back(line[i]);
}

void Raster::fill() {
    unsigned int total_pixels = nrows * ncols;
    for (unsigned int i = 0; i < total_pixels; ++i) pixels.push_back(0);
}

int& Raster::operator()(const int& row, const int& col) {
    assert(row >= 0 && row < nrows);
    assert(col >= 0 && col < ncols);
    return pixels[col + row * ncols];
}

int Raster::operator()(const int& row, const int& col) const {
    assert(row >= 0 && row < nrows);
    assert(col >= 0 && col < ncols);
    return pixels[col + row * ncols];
}

void Raster::set_value(const int& row, const int& col, const int& value) {
    assert(row >= 0 && row < nrows);
    assert(col >= 0 && col < ncols);
    pixels[col + row * ncols] = value;
}


// RasterCell


RasterCell::RasterCell(const int& c_row, const int& c_col, const int& elevation, const int& insertion_order)
    :row(c_row), col(c_col), elevation(elevation), insertion_order(insertion_order) {}

bool RasterCell::operator<(const RasterCell& other) const {
    return ((other.elevation) < (this->elevation)) ||
        ((other.elevation == this->elevation) && (other.insertion_order < this->insertion_order));
    // insertion_order has to be unique;
}
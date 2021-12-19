/*********************************************************
 * The source code is inspired by the skeleton:
 * https://gitlab.tudelft.nl/3d/geo1015.2021/
 *
 * Original author:
 * Ken Arroyo Ohori
 * https://3d.bk.tudelft.nl/ken/en/
 *
 *********************************************************
 *
 * The whole project and related files are available at:
 * https://github.com/SEUZFY/assignment2
 * If a bug is found or there is a better way to implement it, please do not hesitate to contact:
 * F.Zhang-12@student.tudelft.nl(or zmocheng@gmail.com)
 *
 *********************************************************/

#include "Raster.h"
#include <cassert>

// Raster
/*
Raster::Raster(const int& rows, const int& cols)
    : nrows(rows), ncols(cols) {
    unsigned int total_pixels = rows * cols; //unsigned int for big size
    pixels.reserve(total_pixels);
}

void Raster::add_scanline(const int* line) {
    for (int i = 0; i < ncols; ++i) pixels.emplace_back(line[i]);
}

void Raster::fill() {
    unsigned int total_pixels = nrows * ncols;
    for (unsigned int i = 0; i < total_pixels; ++i) pixels.emplace_back(0);
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
}*/

// RasterCell

RasterCell::RasterCell(const int& c_row, const int& c_col, const int& elevation)
    :row(c_row), col(c_col), elevation(elevation), insertion_order(0),
    visited(false), listed(false), direction(0), accumulation(0){}

bool RasterCell::operator<(const RasterCell& other) const {
    return ((other.elevation) < (this->elevation)) ||
        ((other.elevation == this->elevation) && (other.insertion_order < this->insertion_order));
    // insertion_order has to be unique;
}

// ProRaster: Raster with RasterCells

ProRaster::ProRaster(const int& rows, const int& cols)
    :nrows(rows), ncols(cols) {
    unsigned int total_pixels = rows * cols; //unsigned int for big size
    propixels.reserve(total_pixels);
}


RasterCell& ProRaster::operator()(const int& row, const int& col)
{
    assert(row >= 0 && row < nrows);
    assert(col >= 0 && col < ncols);
    return propixels[col + row * ncols];
}


void ProRaster::add_scanline(int& current_line, const int* line)
{
    for (int i = 0; i < ncols; ++i)propixels.emplace_back(RasterCell(current_line, i, line[i]));
}


void ProRaster::output_direction(int& current_line, unsigned int* line)
{
    for (int i = 0; i < ncols; ++i) line[i] = propixels[i + current_line*ncols].direction;
}


void ProRaster::output_accumulation(int& current_line, unsigned int* line)
{
    for (int i = 0; i < ncols; ++i) line[i] = propixels[i + current_line * ncols].accumulation;
}


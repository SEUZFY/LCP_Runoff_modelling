#pragma once
#include <queue>
#include "Raster.h"

//Functions realted to compute flow directions.

constexpr auto SCALE = 0.707107; // 1/(2^1/2);

// add boundary pixels into priority_queue, cost: elevation, if same then insertion_order
void add_outlets_boundary(int& Nrows, int& Ncols, const Raster& r,
	std::priority_queue<RasterCell, std::deque<RasterCell>>& myqueue, int& order);

// return the types(based on numbers) of adjacent pixels of the current pixel r(i,j)
int adjacent_pixel_types(const int& row, const int& col, const Raster& r);

// add neighbours of the processing cell to the priority queue
void add_neighbours(int& i, int& j, Raster& r,
	std::priority_queue<RasterCell, std::deque<RasterCell>>& myqueue, int& order);

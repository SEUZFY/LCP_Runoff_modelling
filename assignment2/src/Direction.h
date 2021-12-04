#pragma once
#include <queue>
#include "Raster.h"

//Functions realted to compute flow directions.

// add boundary pixels into priority_queue, cost: elevation, if same then insertion_order
void add_outlets_boundary(int& Nrows, int& Ncols, ProRaster& r,
	std::priority_queue<RasterCell, std::deque<RasterCell>>& myqueue, int& order);

// return the types(based on numbers) of adjacent pixels of the current pixel r(i,j)
int adjacent_pixel_types(const int& row, const int& col, ProRaster& r);

// add neighbours of the processing cell to the priority queue
void add_neighbours(int& i, int& j, ProRaster& r,
	std::priority_queue<RasterCell, std::deque<RasterCell>>& myqueue, int& order);

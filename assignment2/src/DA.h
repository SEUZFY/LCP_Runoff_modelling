#pragma once

#include <queue>
#include "Raster.h"
#include "gdal_priv.h"

//Functions realted to compute flow directions.

// add boundary pixels into priority_queue, cost: elevation, if same then insertion_order
void add_outlets_boundary(const int& Nrows, const int& Ncols, ProRaster& r,
	std::priority_queue<RasterCell, std::deque<RasterCell>>& myqueue, int& order);

// return the types(based on numbers) of adjacent pixels of the current pixel r(i,j)
int adjacent_pixel_types(const int& row, const int& col, const int& Nrows, const int& Ncols);

// add neighbours of the processing cell to the priority queue
void add_neighbours(const int& i, const int& j, ProRaster& r,
	std::priority_queue<RasterCell, std::deque<RasterCell>>& myqueue, int& order);

// compute every direction of each cell
void compute_flow_direction(ProRaster& r,
	std::priority_queue<RasterCell, std::deque<RasterCell>>& myqueue, 
	std::vector<RasterCell>& cellsvector, int& order);

// compute every accumulation of each cell
void compute_flow_accumulation(ProRaster& r, std::vector<RasterCell>& cell_vector);

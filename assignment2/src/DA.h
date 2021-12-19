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

#ifndef _DA_H_
#define _DA_H_

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

#endif
#include <queue>
#include "Raster.h"
#include "RasterCell.h"
#include "Direction.h"

void add_outlets_boundary(int& Nrows, int& Ncols, const Raster& r,
	std::priority_queue<RasterCell, std::deque<RasterCell>>& myqueue)
{
    int order(0);
    for (int j = 0; j != Ncols; ++j) {
        order += 1;
        myqueue.push(RasterCell(0, j, r(0, j), order)); //top
    }
    for (int i = 1; i != Nrows; ++i) {
        order += 1;
       myqueue.push(RasterCell(i, Ncols - 1, r(i, Ncols - 1), order)); //rightmost
    }
    for (int j = Ncols - 2; j != -1; --j) {
        order += 1;
        myqueue.push(RasterCell(Nrows - 1, j, r(Nrows - 1, j), order)); //bottom
    }
    for (int i = Nrows - 2; i != 0; --i) {
        order += 1;
        myqueue.push(RasterCell(i, 0, r(i, 0), order)); //leftmost
    }
}


int adjacent_pixels(int& row, int& col, const Raster& r)
{
    /*
    possible values: 3,5,8
    but for 3,5, multiple traverse orders, how to specify?
    */
    return 0;
}


void compute_direction(Raster& r, std::priority_queue<RasterCell, std::deque<RasterCell>>& myqueue)
{
    //first identify the location of the pixel: boundary or not

}

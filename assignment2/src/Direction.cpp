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


int adjacent_pixel_types(const int& row, const int& col, const Raster& r)
{
    /*
    possible adjacent pixel numbers: 3,5,8
    if index our of the range, return 0
    based on the numbers, specify the types
    */
    
    if (row > (r.nrows - 1) || col > (r.ncols - 1))return 0; //if index is inappropriate
    int Nrows(r.nrows), Ncols(r.ncols);

    // each cell has 3 adjacent pixels, encoding order: clockwise(1-2-3-4)
    if (row == col && row == 0)return 31; //up-left 
    if (row == 0 && col == (Ncols - 1))return 32; //up-right
    if (row == (Nrows - 1) && col == (Ncols - 1))return 33; //low-right
    if (row == (Nrows - 1) && col == 0)return 34; //low-left

    // each cell has 5 adjacent pixels
    if (row == 0 && col > 0 && col < (Ncols - 1))return 51; //top row
    if (col == Ncols - 1 && row > 0 && row < (Nrows - 1))return 52; //rightmost row
    if (row == (Nrows - 1) && col > 0 && col < (Ncols - 1))return 53; //bottom row
    if (col == 0 && row > 0 && row < (Nrows - 1))return 54; //leftmost row

    //each cell has 8 adjacent pixels
    return 8; 
}


void compute_direction(const int& row, const int& col, Raster& r, 
    std::priority_queue<RasterCell, std::deque<RasterCell>>& myqueue)
{
    int type(adjacent_pixel_types(row, col, r));

    switch (type){
    case 0:


        break;
    case 1:


        break;
    case 2:


        break;
    case 3:


        break;
    default:

        break;
    }

}

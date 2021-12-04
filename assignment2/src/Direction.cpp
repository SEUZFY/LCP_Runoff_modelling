#include <queue>
#include "Raster.h"
#include "Direction.h"


void add_outlets_boundary(const int& Nrows, const int& Ncols, ProRaster& r,
	std::priority_queue<RasterCell, std::deque<RasterCell>>& myqueue, int& order)
{
    for (int j = 0; j != Ncols; ++j) {
        r(0, j).insertion_order = ++order;
        r(0, j).listed = true;
        myqueue.emplace(r(0, j)); //top
    }
    for (int i = 1; i != Nrows; ++i) {
        r(i, Ncols - 1).insertion_order = ++order;
        r(i, Ncols - 1).listed = true;
        myqueue.emplace(r(i, Ncols - 1)); //rightmost
    }
    for (int j = Ncols - 2; j != -1; --j) {
        r(Nrows - 1, j).insertion_order = ++order;
        r(Nrows - 1, j).listed = true;
        myqueue.emplace(r(Nrows - 1, j)); //bottom
    }
    for (int i = Nrows - 2; i != 0; --i) {
        r(i, 0).insertion_order = ++order;
        r(i, 0).listed = true;
        myqueue.emplace(r(i, 0)); //leftmost
    }
}


int adjacent_pixel_types(const int& row, const int& col, const int& Nrows, const int& Ncols)
{
    /*
    possible adjacent pixel numbers: 3,5,8
    if index our of the range, return 0
    based on the numbers, specify the types
    */
    
    if (row > (Nrows - 1) || col > (Ncols - 1) || row < 0 || col < 0)return 0; //if index is inappropriate

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

void add_neighbours(const int& i, const int& j, ProRaster& r, 
    std::priority_queue<RasterCell, std::deque<RasterCell>>& myqueue, int& order)
{
    int loc(adjacent_pixel_types(i, j, r.nrows, r.ncols)); // get the location of the processing cell
    switch (loc){
    
    case 31: {
        if (r(i, j + 1).listed == false && r(i, j + 1).visited == false)
        {
            r(i, j + 1).insertion_order = ++order;
            r(i, j + 1).listed = true;
            myqueue.emplace(r(i, j + 1));
        }// right neighbour

        if (r(i + 1, j + 1).listed == false && r(i + 1, j + 1).visited == false)
        {
            r(i + 1, j + 1).insertion_order = ++order;
            r(i + 1, j + 1).listed = true;
            myqueue.emplace(r(i + 1, j + 1));
        }// diagonal neighbour

        if (r(i + 1, j).listed == false && r(i + 1, j).visited == false)
        {
            r(i + 1, j).insertion_order = ++order;
            r(i + 1, j).listed = true;
            myqueue.emplace(r(i + 1, j));
        }// diagonal neighbour
      
        break;
    } //up-left corner

    case 32: {
        if (r(i + 1, j).listed == false && r(i + 1, j).visited == false)
        {
            r(i + 1, j).insertion_order = ++order;
            r(i + 1, j).listed = true;
            myqueue.emplace(r(i + 1, j));
        }// bottom neighbour

        if (r(i + 1, j - 1).listed == false && r(i + 1, j - 1).visited == false)
        {
            r(i + 1, j - 1).insertion_order = ++order;
            r(i + 1, j - 1).listed = true;
            myqueue.emplace(r(i + 1, j - 1));
        }// diagonal neighbour

        if (r(i, j - 1).listed == false && r(i, j - 1).visited == false)
        {
            r(i, j - 1).insertion_order = ++order;
            r(i, j - 1).listed = true;
            myqueue.emplace(r(i, j - 1));
        }// left neighbour

        break;
    } //up-right corner

    case 33: {
        if (r(i, j - 1).listed == false && r(i, j - 1).visited == false)
        {
            r(i, j - 1).insertion_order = ++order;
            r(i, j - 1).listed = true;
            myqueue.emplace(r(i, j - 1));
        }// left neighbour

        if (r(i - 1, j - 1).listed == false && r(i - 1, j - 1).visited == false)
        {
            r(i - 1, j - 1).insertion_order = ++order;
            r(i - 1, j - 1).listed = true;
            myqueue.emplace(r(i - 1, j - 1));
        }// diagonal neighbour

        if (r(i - 1, j).listed == false && r(i - 1, j).visited == false)
        {
            r(i - 1, j).insertion_order = ++order;
            r(i - 1, j).listed = true;
            myqueue.emplace(r(i - 1, j));
        }// top neighbour

        break;
    } // low-right corner

    case 34: {
        if (r(i - 1, j).listed == false && r(i - 1, j).visited == false)
        {
            r(i - 1, j).insertion_order = ++order;
            r(i - 1, j).listed = true;
            myqueue.emplace(r(i - 1, j));
        }// top neighbour

        if (r(i - 1, j + 1).listed == false && r(i - 1, j + 1).visited == false)
        {
            r(i - 1, j + 1).insertion_order = ++order;
            r(i - 1, j + 1).listed = true;
            myqueue.emplace(r(i - 1, j + 1));
        }// diagonal neighbour

        if (r(i, j + 1).listed == false && r(i, j + 1).visited == false)
        {
            r(i, j + 1).insertion_order = ++order;
            r(i, j + 1).listed = true;
            myqueue.emplace(r(i, j + 1));
        }// right neighbour

        break;
    } // low-left corner

    case 51: {
        if (r(i, j + 1).listed == false && r(i, j + 1).visited == false)
        {
            r(i, j + 1).insertion_order = ++order;
            r(i, j + 1).listed = true;
            myqueue.emplace(r(i, j + 1));
        }// right neighbour

        if (r(i + 1, j + 1).listed == false && r(i + 1, j + 1).visited == false)
        {
            r(i + 1, j + 1).insertion_order = ++order;
            r(i + 1, j + 1).listed = true;
            myqueue.emplace(r(i + 1, j + 1));
        }// diagonal neighbour

        if (r(i + 1, j).listed == false && r(i + 1, j).visited == false)
        {
            r(i + 1, j).insertion_order = ++order;
            r(i + 1, j).listed = true;
            myqueue.emplace(r(i + 1, j));
        }// bottom neighbour

        if (r(i + 1, j - 1).listed == false && r(i + 1, j - 1).visited == false)
        {
            r(i + 1, j - 1).insertion_order = ++order;
            r(i + 1, j - 1).listed = true;
            myqueue.emplace(r(i + 1, j - 1));
        }// diagonal neighbour

        if (r(i, j - 1).listed == false && r(i, j - 1).visited == false)
        {
            r(i, j - 1).insertion_order = ++order;
            r(i, j - 1).listed = true;
            myqueue.emplace(r(i, j - 1));
        }// left neighbour

        break;
    } // top row

    case 52: {
        if (r(i + 1, j).listed == false && r(i + 1, j).visited == false)
        {
            r(i + 1, j).insertion_order = ++order;
            r(i + 1, j).listed = true;
            myqueue.emplace(r(i + 1, j));
        }// bottom neighbour

        if (r(i + 1, j - 1).listed == false && r(i + 1, j - 1).visited == false)
        {
            r(i + 1, j - 1).insertion_order = ++order;
            r(i + 1, j - 1).listed = true;
            myqueue.emplace(r(i + 1, j - 1));
        }// diagonal neighbour

        if (r(i, j - 1).listed == false && r(i, j - 1).visited == false)
        {
            r(i, j - 1).insertion_order = ++order;
            r(i, j - 1).listed = true;
            myqueue.emplace(r(i, j - 1));
        }// left neighbour

        if (r(i - 1, j - 1).listed == false && r(i - 1, j - 1).visited == false)
        {
            r(i - 1, j - 1).insertion_order = ++order;
            r(i - 1, j - 1).listed = true;
            myqueue.emplace(r(i - 1, j - 1));
        }// diagonal neighbour

        if (r(i - 1, j).listed == false && r(i - 1, j).visited == false)
        {
            r(i - 1, j).insertion_order = ++order;
            r(i - 1, j).listed = true;
            myqueue.emplace(r(i - 1, j));
        }// top neighbour

        break;
    } // rightmost row



    default: // loc = 0 outside of the boundary
        break;
    }


}


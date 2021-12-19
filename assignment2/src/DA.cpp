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

#include <iostream>
#include "DA.h"

// #include <queue>
// #include "Raster.h"


void add_outlets_boundary(const int& Nrows, const int& Ncols, ProRaster& r,
	std::priority_queue<RasterCell, std::deque<RasterCell>>& myqueue, int& order)
{
    for (int j = 0; j != Ncols; ++j) {
        r(0, j).insertion_order = ++order;
        r(0, j).listed = true;
        myqueue.emplace(r(0, j)); // top
    }
    for (int i = 1; i != Nrows; ++i) {
        r(i, Ncols - 1).insertion_order = ++order;
        r(i, Ncols - 1).listed = true;
        myqueue.emplace(r(i, Ncols - 1)); // rightmost
    }
    for (int j = Ncols - 2; j != -1; --j) {
        r(Nrows - 1, j).insertion_order = ++order;
        r(Nrows - 1, j).listed = true;
        myqueue.emplace(r(Nrows - 1, j)); // bottom
    }
    for (int i = Nrows - 2; i != 0; --i) {
        r(i, 0).insertion_order = ++order;
        r(i, 0).listed = true;
        myqueue.emplace(r(i, 0)); // leftmost
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
    else if (row == col && row == 0)return 31; // up-left 
    else if (row == 0 && col == (Ncols - 1))return 32; // up-right
    else if (row == (Nrows - 1) && col == (Ncols - 1))return 33; // low-right
    else if (row == (Nrows - 1) && col == 0)return 34; // low-left

    // each cell has 5 adjacent pixels
    else if (row == 0 && col > 0 && col < (Ncols - 1))return 51; // top row
    else if (col == Ncols - 1 && row > 0 && row < (Nrows - 1))return 52; // rightmost row
    else if (row == (Nrows - 1) && col > 0 && col < (Ncols - 1))return 53; // bottom row
    else if (col == 0 && row > 0 && row < (Nrows - 1))return 54; // leftmost row

    // each cell has 8 adjacent pixels
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
        }// bottom neighbour
      
        break;
    } // up-left corner

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
    } // up-right corner

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

    case 53: {
        if (r(i, j - 1).listed == false && r(i, j - 1).visited == false)
        {
            r(i, j - 1).insertion_order = ++order;
            r(i, j - 1).listed = true;
            myqueue.emplace(r(i, j - 1));
        } // left neighbour

        if (r(i - 1, j - 1).listed == false && r(i - 1, j - 1).visited == false)
        {
            r(i - 1, j - 1).insertion_order = ++order;
            r(i - 1, j - 1).listed = true;
            myqueue.emplace(r(i - 1, j - 1));
        } // diagonal neighbour

        if (r(i - 1, j).listed == false && r(i - 1, j).visited == false)
        {
            r(i - 1, j).insertion_order = ++order;
            r(i - 1, j).listed = true;
            myqueue.emplace(r(i - 1, j));
        } // top neighbour

        if (r(i - 1, j + 1).listed == false && r(i - 1, j + 1).visited == false)
        {
            r(i - 1, j + 1).insertion_order = ++order;
            r(i - 1, j + 1).listed = true;
            myqueue.emplace(r(i - 1, j + 1));
        } // diagonal neighbour

        if (r(i, j + 1).listed == false && r(i, j + 1).visited == false)
        {
            r(i, j + 1).insertion_order = ++order;
            r(i, j + 1).listed = true;
            myqueue.emplace(r(i, j + 1));
        } // right neighbour

        break;
    } // bottom row

    case 54: {
        if (r(i - 1, j).listed == false && r(i - 1, j).visited == false)
        {
            r(i - 1, j).insertion_order = ++order;
            r(i - 1, j).listed = true;
            myqueue.emplace(r(i - 1, j));
        } // top neighbour

        if (r(i - 1, j + 1).listed == false && r(i - 1, j + 1).visited == false)
        {
            r(i - 1, j + 1).insertion_order = ++order;
            r(i - 1, j + 1).listed = true;
            myqueue.emplace(r(i - 1, j + 1));
        } // diagonal neighbour

        if (r(i, j + 1).listed == false && r(i, j + 1).visited == false)
        {
            r(i, j + 1).insertion_order = ++order;
            r(i, j + 1).listed = true;
            myqueue.emplace(r(i, j + 1));
        } // right neighbour

        if (r(i + 1, j + 1).listed == false && r(i + 1, j + 1).visited == false)
        {
            r(i + 1, j + 1).insertion_order = ++order;
            r(i + 1, j + 1).listed = true;
            myqueue.emplace(r(i + 1, j + 1));
        } // diagonal neighbour

        if (r(i + 1, j).listed == false && r(i + 1, j).visited == false)
        {
            r(i + 1, j).insertion_order = ++order;
            r(i + 1, j).listed = true;
            myqueue.emplace(r(i + 1, j));
        } // bottom neighbour

        break;
    } // leftmost row

    case 8: {
        if (r(i - 1, j - 1).listed == false && r(i - 1, j - 1).visited == false)
        {
            r(i - 1, j - 1).insertion_order = ++order;
            r(i - 1, j - 1).listed = true;
            myqueue.emplace(r(i - 1, j - 1));
        } // diagonal neighbour

        if (r(i - 1, j).listed == false && r(i - 1, j).visited == false)
        {
            r(i - 1, j).insertion_order = ++order;
            r(i - 1, j).listed = true;
            myqueue.emplace(r(i - 1, j));
        } // top neighbour

        if (r(i - 1, j + 1).listed == false && r(i - 1, j + 1).visited == false)
        {
            r(i - 1, j + 1).insertion_order = ++order;
            r(i - 1, j + 1).listed = true;
            myqueue.emplace(r(i - 1, j + 1));
        } // diagonal neighbour

        if (r(i, j + 1).listed == false && r(i, j + 1).visited == false)
        {
            r(i, j + 1).insertion_order = ++order;
            r(i, j + 1).listed = true;
            myqueue.emplace(r(i, j + 1));
        } // right neighbour

        if (r(i + 1, j + 1).listed == false && r(i + 1, j + 1).visited == false)
        {
            r(i + 1, j + 1).insertion_order = ++order;
            r(i + 1, j + 1).listed = true;
            myqueue.emplace(r(i + 1, j + 1));
        } // diagonal neighbour

        if (r(i + 1, j).listed == false && r(i + 1, j).visited == false)
        {
            r(i + 1, j).insertion_order = ++order;
            r(i + 1, j).listed = true;
            myqueue.emplace(r(i + 1, j));
        } // bottom neighbour

        if (r(i + 1, j - 1).listed == false && r(i + 1, j - 1).visited == false)
        {
            r(i + 1, j - 1).insertion_order = ++order;
            r(i + 1, j - 1).listed = true;
            myqueue.emplace(r(i + 1, j - 1));
        } // diagonal neighbour

        if (r(i, j - 1).listed == false && r(i, j - 1).visited == false)
        {
            r(i, j - 1).insertion_order = ++order;
            r(i, j - 1).listed = true;
            myqueue.emplace(r(i, j - 1));
        } // left neighbour

        break;
    } // cells with 8 neighbours

    default: // loc = 0 outside of the boundary
        break;
    }

}


void compute_flow_direction(ProRaster& r, 
    std::priority_queue<RasterCell, std::deque<RasterCell>>& myqueue, 
    std::vector<RasterCell>& cellsvector, int& order)
{
    while (!myqueue.empty())
    {
        // process the current cell
        int i(myqueue.top().row), j(myqueue.top().col); // row and col of processing cell
        r(i, j).visited = true; // current cell: visited = true
        myqueue.pop();

        // set directions of its neighbours
        int loc(adjacent_pixel_types(i, j, r.nrows, r.ncols));
        switch (loc){
        
        case 31: {
            // right neighbour
            if (!r(i, j + 1).direction)r(i, j + 1).direction = 16;
            
            // diagonal neighbour
            if (!r(i + 1, j + 1).direction)r(i + 1, j + 1).direction = 32;
            
            // bottom neighbour
            if (!r(i + 1, j).direction)r(i + 1, j).direction = 64;

            break;
        } // up-left corner

        case 32: {
            // bottom neighbour
            if (!r(i + 1, j).direction)r(i + 1, j).direction = 64;
            
            // diagonal neighbour
            if (!r(i + 1, j - 1).direction)r(i + 1, j - 1).direction = 128;
            
            // left neighbour
            if (!r(i, j - 1).direction)r(i, j - 1).direction = 1;
            
            break;
        } // up-right corner

        case 33: {
            // left neighbour
            if (!r(i, j - 1).direction)r(i, j - 1).direction = 1;
            
            // diagonal neighbour
            if (!r(i - 1, j - 1).direction)r(i - 1, j - 1).direction = 2;
            
            // top neighbour
            if (!r(i - 1, j).direction)r(i - 1, j).direction = 4;

            break;
        } // low-right corner

        case 34: {
            // top neighbour
            if (!r(i - 1, j).direction)r(i - 1, j).direction = 4;
            
            // diagonal neighbour
            if (!r(i - 1, j + 1).direction)r(i - 1, j + 1).direction = 8;
            
            // right neighbour
            if (!r(i, j + 1).direction)r(i, j + 1).direction = 16;

            break;
        } // low-left corner

        case 51: {
            // right neighbour
            if (!r(i, j + 1).direction)r(i, j + 1).direction = 16;
            
            // diagonal neighbour
            if (!r(i + 1, j + 1).direction)r(i + 1, j + 1).direction = 32;
            
            // bottom neighbour
            if (!r(i + 1, j).direction)r(i + 1, j).direction = 64;
            
            // diagonal neighbour
            if (!r(i + 1, j - 1).direction)r(i + 1, j - 1).direction = 128;
            
            // left neighbour
            if (!r(i, j - 1).direction)r(i, j - 1).direction = 1;
            
            break;
        } // top row

        case 52: {
            // bottom neighbour
            if (!r(i + 1, j).direction)r(i + 1, j).direction = 64;
            
            // diagonal neighbour
            if (!r(i + 1, j - 1).direction)r(i + 1, j - 1).direction = 128;
            
            // left neighbour
            if (!r(i, j - 1).direction)r(i, j - 1).direction = 1;
            
            // diagonal neighbour
            if (!r(i - 1, j - 1).direction)r(i - 1, j - 1).direction = 2;
            
            // top neighbour
            if (!r(i - 1, j).direction)r(i - 1, j).direction = 4;
            
            break;
        } // rightmost row
        
        case 53: {
            // left neighbour
            if (!r(i, j - 1).direction)r(i, j - 1).direction = 1;
            
            // diagonal neighbour
            if (!r(i - 1, j - 1).direction)r(i - 1, j - 1).direction = 2;
            
            // top neighbour
            if (!r(i - 1, j).direction)r(i - 1, j).direction = 4;
            
            // diagonal neighbour
            if (!r(i - 1, j + 1).direction)r(i - 1, j + 1).direction = 8;
            
            // right neighbour
            if (!r(i, j + 1).direction)r(i, j + 1).direction = 16;
            
            break;
        } // bottom row

        case 54: {
            // top neighbour
            if (!r(i - 1, j).direction)r(i - 1, j).direction = 4;
            
            // diagonal neighbour
            if (!r(i - 1, j + 1).direction)r(i - 1, j + 1).direction = 8;
            
            // right neighbour
            if (!r(i, j + 1).direction)r(i, j + 1).direction = 16;
            
            // diagonal neighbour
            if (!r(i + 1, j + 1).direction)r(i + 1, j + 1).direction = 32;
            
            // bottom neighbour
            if (!r(i + 1, j).direction)r(i + 1, j).direction = 64;

            break;
        } // leftmost row

        case 8: {
            // diagonal neighbour
            if (!r(i - 1, j - 1).direction)r(i - 1, j - 1).direction = 2;
      
            // top neighbour
            if (!r(i - 1, j).direction)r(i - 1, j).direction = 4;
            
            // diagonal neighbour
            if (!r(i - 1, j + 1).direction)r(i - 1, j + 1).direction = 8;
            
            // right neighbour
            if (!r(i, j + 1).direction)r(i, j + 1).direction = 16;
            
            // diagonal neighbour
            if (!r(i + 1, j + 1).direction)r(i + 1, j + 1).direction = 32;
            
            // bottom neighbour
            if (!r(i + 1, j).direction)r(i + 1, j).direction = 64;
            
            // diagonal neighbour
            if (!r(i + 1, j - 1).direction)r(i + 1, j - 1).direction = 128;
            
            // left neighbour
            if (!r(i, j - 1).direction)r(i, j - 1).direction = 1;
            
            break;
        } // cells with 8 neighbours

        default:
            break;
        }

        // add the processing cell to the vector(opposite order)
        cellsvector.emplace_back(r(i, j));

        // add the neighbours to the queue
        add_neighbours(i, j, r, myqueue, order);

    }
}


void compute_flow_accumulation(ProRaster& r, std::vector<RasterCell>& cell_vector)
{
    /*the direction of the first cell in cell_vector needs to be assigned because in the
    * compute_flow_direction() function, after the calculation, the direction of the
    first cell in the priority queue is still unknown, yet it is added to the cell_vector
    */

    // assign the direction of the first cell in cell_vector
    cell_vector.front().direction = r(cell_vector.front().row, cell_vector.front().col).direction;
    
    while (!cell_vector.empty()) {
        int i(cell_vector.back().row), j(cell_vector.back().col);
        int direct(r(i, j).direction);

        switch (direct) 
        {
        case 1:
            r(i, j + 1).accumulation += (1 + r(i, j).accumulation);
            break;
        case 2:
            r(i + 1, j + 1).accumulation += (1 + r(i, j).accumulation);
            break;
        case 4:
            r(i + 1, j).accumulation += (1 + r(i, j).accumulation);
            break;
        case 8:
            r(i + 1, j - 1).accumulation += (1 + r(i, j).accumulation);
            break;
        case 16:
            r(i, j - 1).accumulation += (1 + r(i, j).accumulation);
            break;
        case 32:
            r(i - 1, j - 1).accumulation += (1 + r(i, j).accumulation);
            break;
        case 64:
            r(i - 1, j).accumulation += (1 + r(i, j).accumulation);
            break;
        case 128:
            r(i - 1, j + 1).accumulation += (1 + r(i, j).accumulation);
            break;
        default:
            break;
        }
        // pop the processing cell
        cell_vector.pop_back();
    }
}


#include <queue>
#include <cmath>
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


int compute_cell_direction(const int& i, const int& j, Raster& r)
{
    /*
    direction encoding:
    east-1 southeast-2 south-4 southwest-8
    west-16 northwest-32 north-64 northeast-128
    */
    
    int type(adjacent_pixel_types(i, j, r));
    int cur(r(i, j)); //current cell
    double min(0); //maintain a double variable to get the smallest cell
    int result(0);

    switch (type){ 
    case 31: {
        /*
        up-left corner
        clockwise
        r(i, j + 1); East
        r(i + 1, j + 1); Southeast
        r(i + 1, j); South
        */

        double e(r(i, j + 1)), se(r(i + 1, j + 1)), s(r(i + 1, j));
        double slope_e(abs(e - cur)), slope_se(abs(se - cur)*SCALE), slope_s(abs(s - cur));
        
        min = (e < se) ? e : se; // find the smallest cell
        min = min < s ? min : s;

        if (min >= cur) // least steep upwards slope
        {
            double min_slope(0);
            min_slope = (slope_e < slope_se) ? slope_e : slope_se;
            min_slope = min_slope < slope_s ? min_slope : slope_s;

            if (min_slope == slope_e)result = 1;
            else if (min_slope == slope_se)result = 2;
            else if (min_slope == slope_s)result = 4;
        }
        else // steepest downwards slope
        {
            double max_slope(0);
            max_slope = (slope_e > slope_se) ? slope_e : slope_se;
            max_slope = max_slope > slope_s ? max_slope : slope_s;

            if (max_slope == slope_e && min == e)result = 1;
            else if (max_slope == slope_se && min == se)result = 2;
            else if (max_slope == slope_s && min == s)result = 4;

            else if (min == e)result = 1; // if cell of max_slope > current cell, minimum cell is the direction
            else if (min == se)result = 2;
            else if (min == s)result = 4;
        }       
        break;
    }

    case 32: {
        /*
        up-right corner
        clockwise
        r(i+1, j); south
        r(i + 1, j - 1); southwest
        r(i, j-1); west
        */

        double s(r(i+1, j)), sw(r(i + 1, j - 1)), w(r(i, j-1));
        double slope_s(abs(s - cur)), slope_sw(abs(sw - cur) * SCALE), slope_w(abs(w - cur));
        
        min = (s < sw) ? s : sw; // find the smallest cell
        min = min < w ? min : w;
        
        if (min >= cur) // least steep upwards slope
        {
            double min_slope(0);
            min_slope = (slope_s < slope_sw) ? slope_s : slope_sw;
            min_slope = min_slope < slope_w ? min_slope : slope_w;

            if (min_slope == slope_s)result = 4;
            else if (min_slope == slope_sw)result = 8;
            else if (min_slope == slope_w)result = 16;
        }
        else // steepest downwards slope
        {
            double max_slope(0);
            max_slope = (slope_s > slope_sw) ? slope_s : slope_sw;
            max_slope = max_slope > slope_w ? max_slope : slope_w;

            if (max_slope == slope_s && min == s)result = 4;
            else if (max_slope == slope_sw && min == sw)result = 8;
            else if (max_slope == slope_w && min == w)result = 16;

            else if (min == s)result = 4; // if cell of max_slope > current cell, minimum cell is the direction
            else if (min == sw)result = 8;
            else if (min == w)result = 16;
        }
        break;
    }

    case 33: {
        /*
        low-right corner
        clockwise
        r(i, j-1); west
        r(i - 1, j - 1); northwest
        r(i-1, j); north
        */

        double w(r(i, j - 1)), nw(r(i - 1, j - 1)), n(r(i - 1, j));
        double slope_w(abs(w - cur)), slope_nw(abs(nw - cur) * SCALE), slope_n(abs(n - cur));

        min = (w < nw) ? w : nw; // find the smallest cell
        min = min < n ? min : n;

        if (min >= cur) // least steep upwards slope
        {
            double min_slope(0);
            min_slope = (slope_w < slope_nw) ? slope_w : slope_nw;
            min_slope = min_slope < slope_n ? min_slope : slope_n;

            if (min_slope == slope_w)result = 16;
            else if (min_slope == slope_nw)result = 32;
            else if (min_slope == slope_n)result = 64;
        }
        else // steepest downwards slope
        {
            double max_slope(0);
            max_slope = (slope_w > slope_nw) ? slope_w : slope_nw;
            max_slope = max_slope > slope_n ? max_slope : slope_n;

            if (max_slope == slope_w && min == w)result = 16;
            else if (max_slope == slope_nw && min == nw)result = 32;
            else if (max_slope == slope_n && min == n)result = 64;

            else if (min == w)result = 16; // if cell of max_slope > current cell, minimum cell is the direction
            else if (min == nw)result = 32;
            else if (min == n)result = 64;
        }
        break;
    }
    
    case 34: {
        /*
        low-left corner
        clockwise
        r(i-1, j); north
        r(i - 1, j + 1); northeast
        r(i, j+1); east
        */

        double n(r(i - 1, j)), ne(r(i - 1, j + 1)), e(r(i, j + 1));
        double slope_n(abs(n - cur)), slope_ne(abs(ne - cur) * SCALE), slope_e(abs(e - cur));

        min = (n < ne) ? n : ne; // find the smallest cell
        min = min < e ? min : e;

        if (min >= cur) // least steep upwards slope
        {
            double min_slope(0);
            min_slope = (slope_n < slope_ne) ? slope_n : slope_ne;
            min_slope = min_slope < slope_e ? min_slope : slope_e;

            if (min_slope == slope_n)result = 64;
            else if (min_slope == slope_ne)result = 128;
            else if (min_slope == slope_e)result = 1;
        }
        else // steepest downwards slope
        {
            double max_slope(0);
            max_slope = (slope_n > slope_ne) ? slope_n : slope_ne;
            max_slope = max_slope > slope_e ? max_slope : slope_e;

            if (max_slope == slope_n && min == n)result = 64; // cur: 7, neighbor: 8,7,6
            else if (max_slope == slope_ne && min == ne)result = 128; // min == ... 
            else if (max_slope == slope_e && min == e)result = 1; // slope(8) = slope(6) but slope(6) is right

            else if (min == n)result = 64; // if cell of max_slope > current cell, minimum cell is the direction
            else if (min == ne)result = 128;
            else if (min == e)result = 1;
        }
        break;
    }

    case 51: {
        /*
        top row
        clockwise
        r(i, j + 1); east
        r(i + 1, j + 1); southeast
        r(i + 1, j); south
        r(i + 1, j - 1); southwest
        r(i, j - 1); west
        */

        double e(r(i, j + 1)), se(r(i + 1, j + 1)), s(r(i + 1, j)),
            sw(r(i + 1, j - 1)), w(r(i, j - 1));
        double slope_e(abs(e - cur)), slope_se(abs(se - cur)* SCALE), slope_s(abs(s - cur)),
            slope_sw(abs(sw - cur)* SCALE), slope_w(abs(w - cur));

        min = (e < se) ? e : se; // find the smallest cell
        min = min < s ? min : s;
        min = min < sw ? min : sw;
        min = min < w ? min : w;

        if (min >= cur) // least steep upwards slope
        {
            double min_slope(0);
            min_slope = (slope_e < slope_se) ? slope_e : slope_se;
            min_slope = min_slope < slope_s ? min_slope : slope_s;
            min_slope = min_slope < slope_sw ? min_slope : slope_sw;
            min_slope = min_slope < slope_w ? min_slope : slope_w;

            if (min_slope == slope_e)result = 1;
            else if (min_slope == slope_se)result = 2;
            else if (min_slope == slope_s)result = 4;
            else if (min_slope == slope_sw)result = 8;
            else if (min_slope == slope_w)result = 16;
        }
        else // steepest downwards slope
        {
            double max_slope(0);
            max_slope = (slope_e > slope_se) ? slope_e : slope_se;
            max_slope = max_slope > slope_s ? max_slope : slope_s;
            max_slope = max_slope > slope_sw ? max_slope : slope_sw;
            max_slope = max_slope > slope_w ? max_slope : slope_w;

            if (max_slope == slope_e && min == e)result = 1;
            else if (max_slope == slope_se && min == se)result = 2;
            else if (max_slope == slope_s && min == s)result = 4;
            else if (max_slope == slope_sw && min == sw)result = 8;
            else if (max_slope == slope_w && min == w)result = 16;

            else if (min == e)result = 1; // if cell of max_slope > current cell, min cell is the direction
            else if (min == se)result = 2;
            else if (min == s)result = 4;
            else if (min == sw)result = 8;
            else if (min == w)result = 16;
        }
        break;
    }

    case 52: {
        /*
        rightmost row
        clockwise
        r(i + 1, j); south
        r(i + 1, j - 1); southwest
        r(i, j - 1); west
        r(i - 1, j - 1); northwest
        r(i - 1, j); north
        */

        double s(r(i + 1, j)), sw(r(i + 1, j - 1)), w(r(i, j - 1)),
            nw(r(i - 1, j - 1)), n(r(i - 1, j));
        double slope_s(abs(s - cur)), slope_sw(abs(sw - cur) * SCALE), slope_w(abs(w - cur)),
            slope_nw(abs(nw - cur) * SCALE), slope_n(abs(n - cur));

        min = (s < sw) ? s : sw; // find the smallest cell
        min = min < w ? min : w;
        min = min < nw ? min : nw;
        min = min < n ? min : n;

        if (min >= cur) // least steep upwards slope
        {
            double min_slope(0);
            min_slope = (slope_s < slope_sw) ? slope_s : slope_sw;
            min_slope = min_slope < slope_w ? min_slope : slope_w;
            min_slope = min_slope < slope_nw ? min_slope : slope_nw;
            min_slope = min_slope < slope_n ? min_slope : slope_n;

            if (min_slope == slope_s)result = 4;
            else if (min_slope == slope_sw)result = 8;
            else if (min_slope == slope_w)result = 16;
            else if (min_slope == slope_nw)result = 32;
            else if (min_slope == slope_n)result = 64;
        }
        else // steepest downwards slope
        {
            double max_slope(0);
            max_slope = (slope_s > slope_sw) ? slope_s : slope_sw;
            max_slope = max_slope > slope_w ? max_slope : slope_w;
            max_slope = max_slope > slope_nw ? max_slope : slope_nw;
            max_slope = max_slope > slope_n ? max_slope : slope_n;

            if (max_slope == slope_s && min == s)result = 4; // max_slope and meanwhile the minimum cell
            else if (max_slope == slope_sw && min == sw)result = 8;
            else if (max_slope == slope_w && min == w)result = 16;
            else if (max_slope == slope_nw && min == nw)result = 32;
            else if (max_slope == slope_n && min == n)result = 64; 

            else if (min == s)result = 4; // if cell of max_slope > current cell, min cell is the direction
            else if (min == sw)result = 8;
            else if (min == w)result = 16;
            else if (min == nw)result = 32;
            else if (min == n)result = 64;
        }
        break;
    }

    case 53: {
        /*
        bottom row
        clockwise
        r(i, j - 1); west
        r(i - 1, j - 1); northwest
        r(i - 1, j); north
        r(i - 1, j + 1); northeast
        r(i, j + 1); east
        */

        double w(r(i, j - 1)), nw(r(i - 1, j - 1)), n(r(i - 1, j)),
            ne(r(i - 1, j + 1)), e(r(i, j + 1));
        double slope_w(abs(w - cur)), slope_nw(abs(nw - cur) * SCALE), slope_n(abs(n - cur)),
            slope_ne(abs(ne - cur) * SCALE), slope_e(abs(e - cur));

        min = (w < nw) ? w : nw; // find the smallest cell
        min = min < n ? min : n;
        min = min < ne ? min : ne;
        min = min < e ? min : e;

        if (min >= cur) // least steep upwards slope
        {
            double min_slope(0);
            min_slope = slope_w < slope_nw ? slope_w : slope_nw;
            min_slope = min_slope < slope_n ? min_slope : slope_n;
            min_slope = min_slope < slope_ne ? min_slope : slope_ne;
            min_slope = min_slope < slope_e ? min_slope : slope_e;

            if (min_slope == slope_w)result = 16;
            else if (min_slope == slope_nw)result = 32;
            else if (min_slope == slope_n)result = 64;
            else if (min_slope == slope_ne)result = 128;
            else if (min_slope == slope_e)result = 1;
        }
        else // steepest downwards slope
        {
            double max_slope(0);
            max_slope = slope_w > slope_nw ? slope_w : slope_nw;
            max_slope = max_slope > slope_n ? max_slope : slope_n;
            max_slope = max_slope > slope_ne ? max_slope : slope_ne;
            max_slope = max_slope > slope_e ? max_slope : slope_e;

            if (max_slope == slope_w && min == w)result = 16; // max_slope and meanwhile the minimum cell
            else if (max_slope == slope_nw && min == nw)result = 32;
            else if (max_slope == slope_n && min == n)result = 64;
            else if (max_slope == slope_ne && min == ne)result = 128;
            else if (max_slope == slope_e && min == e)result = 1;

            else if (min == w)result = 16; // if cell of max_slope > current cell, min cell is the direction
            else if (min == nw)result = 32;
            else if (min == n)result = 64;
            else if (min == ne)result = 128;
            else if (min == e)result = 1;
        }
        break;
    }

    case 54: {
        /*
        leftmost row
        clockwise
        r(i - 1, j); north
        r(i - 1, j + 1); northeast
        r(i, j + 1); east
        r(i + 1, j + 1); southeast
        r(i + 1, j); south
        */

        double n(r(i - 1, j)), ne(r(i - 1, j + 1)), e(r(i, j + 1)),
            se(r(i + 1, j + 1)), s(r(i + 1, j));
        double slope_n(abs(n - cur)), slope_ne(abs(ne - cur) * SCALE), slope_e(abs(e - cur)),
            slope_se(abs(se - cur) * SCALE), slope_s(abs(s - cur));

        min = n < ne ? n : ne; // find the smallest cell
        min = min < e ? min : e;
        min = min < se ? min : se;
        min = min < s ? min : s;

        if (min >= cur) // least steep upwards slope
        {
            double min_slope(0);
            min_slope = slope_n < slope_ne ? slope_n : slope_ne;
            min_slope = min_slope < slope_e ? min_slope : slope_e;
            min_slope = min_slope < slope_se ? min_slope : slope_se;
            min_slope = min_slope < slope_s ? min_slope : slope_s;

            if (min_slope == slope_n)result = 64;
            else if (min_slope == slope_ne)result = 128;
            else if (min_slope == slope_e)result = 1;
            else if (min_slope == slope_se)result = 2;
            else if (min_slope == slope_s)result = 4;
        }
        else // steepest downwards slope
        {
            double max_slope(0);
            max_slope = slope_n > slope_ne ? slope_n : slope_ne;
            max_slope = max_slope > slope_e ? max_slope : slope_e;
            max_slope = max_slope > slope_se ? max_slope : slope_se;
            max_slope = max_slope > slope_s ? max_slope : slope_s;

            if (max_slope == slope_n && min == n)result = 64; // max_slope and meanwhile the minimum cell
            else if (max_slope == slope_ne && min == ne)result = 128;
            else if (max_slope == slope_e && min == e)result = 1;
            else if (max_slope == slope_se && min == se)result = 2;
            else if (max_slope == slope_s && min == s)result = 4;

            else if (min == n)result = 64; // if cell of max_slope > current cell, min cell is the direction
            else if (min == ne)result = 128;
            else if (min == e)result = 1;
            else if (min == se)result = 2;
            else if (min == s)result = 4;
        }
        break;
    }
       
    default: // type = 0

       
        break;
    }
    return result;
}

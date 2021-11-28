#include "RasterCell.h"

RasterCell::RasterCell(int c_row, int c_col, int elevation, int insertion_order) 
	:row(c_row), col(c_col), elevation(elevation), insertion_order(insertion_order) {}

bool RasterCell::operator<(const RasterCell& other) const {
    return (other.insertion_order) > (this->insertion_order) ? true : false;
}
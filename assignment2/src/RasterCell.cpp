#include "RasterCell.h"

RasterCell::RasterCell(const int& c_row, const int& c_col, const int& elevation, const int& insertion_order)
	:row(c_row), col(c_col), elevation(elevation), insertion_order(insertion_order) {}

bool RasterCell::operator<(const RasterCell& other) const {
	return ((other.elevation) < (this->elevation)) ||
		((other.elevation == this->elevation) && (other.insertion_order < this->insertion_order));
	// insertion_order has to be unique;
}
#include <iostream>

#include "image.h"
#include "io_tools.h"

igg::Image::Image() {};
igg::Image::Image(int rows, int cols): rows_{rows}, cols_{cols} {
	data_.resize(rows * cols);
}
int igg::Image::rows() const {return rows_;}
int igg::Image::cols() const {return cols_;}
int& igg::Image::at(int row, int col) {
	return data_.at(row * rows_ + col);
}

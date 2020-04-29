#include <iostream>

#include "image.h"

igg::Image::Image(int rows, int cols, const IoStrategy& io_strategy):  rows_{rows}, cols_{cols}, io_strategy_{io_strategy} {
	data_.resize(rows * cols);
}
igg::Image::Image(const IoStrategy& io_strategy):  io_strategy_{io_strategy} {}
int igg::Image::rows() const {return rows_;}
int igg::Image::cols() const {return cols_;}
igg::Image::Pixel& igg::Image::at(int row, int col) {
	return data_.at(row * rows_ + col);
}

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
void igg::Image::DownScale(int scale) {
	int new_rows = (rows_ - 1) / scale + 1;
	int new_cols = (cols_ - 1) / scale + 1;
	std::vector<Pixel> new_data(new_rows * new_cols);
	for (int i = 0; i < new_rows; i++) {
		for (int j = 0; j < new_cols; j++) {
			new_data.at(i * new_rows + j) = data_.at(i * scale * rows_ + j * scale);
		}
	}
	rows_ = new_rows;
	cols_ = new_cols;
	data_ = new_data;
}

void igg::Image::UpScale(int scale) {
	int new_rows = rows_ * scale;
	int new_cols = cols_ * scale;
	std::vector<Pixel> new_data(new_rows * new_cols);
	int nearest_row, nearest_col;
	for (int i = 0; i < new_rows; i++) {
		for (int j = 0; j < new_cols; j++) {
			nearest_row = i / scale;
			if (i % scale > scale / 2 && nearest_row + 1 < rows_) {
				++nearest_row;
			}
			nearest_col = j / scale;
			if (j % scale > scale / 2 && nearest_col + 1 < cols_) {
				++nearest_col;
			}
			new_data.at(i * new_rows + j) = data_.at(nearest_row * rows_ + nearest_col);
		}
	}
	rows_ = new_rows;
	cols_ = new_cols;
	data_ = new_data;
}

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
bool igg::Image::FillFromPgm(const std::string& file_name) {
	igg::io_tools::ImageData image_data = igg::io_tools::ReadFromPgm(file_name);
	rows_ = image_data.rows;
	cols_ = image_data.cols;
	max_val_ = image_data.max_val;
	data_ = image_data.data;
	return true;
}
bool igg::Image::WriteToPgm(const std::string& file_name) {
	igg::io_tools::ImageData image_data{rows_, cols_, max_val_, data_};
	return igg::io_tools::WriteToPgm(image_data, file_name);
}

std::vector<float> igg::Image::ComputeHistogram(int bins) const {
	int bin_size = max_val_ / bins;
	int n_pixels = rows_ * cols_;
	std::vector<float> bin_count(bins);
	for (auto elem : data_) {
		++bin_count[elem / bin_size < bins ? elem / bin_size : bins - 1];
	}
	for (auto& elem : bin_count) {
		elem /= n_pixels;
	}
	return bin_count;
}

void igg::Image::DownScale(int scale) {
	int new_rows = (rows_ - 1) / scale + 1;
	int new_cols = (cols_ - 1) / scale + 1;
	std::vector<int> new_data(new_rows * new_cols);
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
	std::vector<int> new_data(new_rows * new_cols);
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

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

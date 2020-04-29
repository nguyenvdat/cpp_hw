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

bool igg::Image::ReadFromDisk(const std::string& file_name) {
	auto image_data = io_strategy_.Read(file_name);
	rows_ = image_data.rows;
	cols_ = image_data.cols;
	max_val_ = image_data.max_val;
	data_.resize(rows_ * cols_);
	auto data = image_data.data;
	for (int i = 0; i < data_.size(); ++i) {
		Pixel pixel{data[0][i], data[1][i], data[2][i]};
		data_[i] = pixel;
	}
	return true;
}

void igg::Image::WriteToDisk(const std::string& file_name) {
	std::vector<std::vector<int>> data;
	std::vector<int> red_vector;
	std::vector<int> blue_vector;
	std::vector<int> green_vector;
	for (auto elem : data_) {
		red_vector.push_back(elem.red);
		blue_vector.push_back(elem.blue);
		green_vector.push_back(elem.green);
	}
	data.push_back(red_vector);
	data.push_back(blue_vector);
	data.push_back(green_vector);
	ImageData image_data{rows_, cols_, max_val_, data};
	io_strategy_.Write(file_name, image_data);
}

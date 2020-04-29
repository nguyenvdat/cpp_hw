#include "igg_image/io_strategies/ppm_strategy.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

namespace igg {

bool PpmIoStrategy::Write(const std::string& file_name,
                          const ImageData& data) const {
	std::ofstream f(file_name, std::ios_base::out
	                | std::ios_base::binary
	                | std::ios_base::trunc
	               );
	f << "PS3\n" << data.rows << " " << data.cols << "\n"  << data.max_val << "\n";
	const int MAX_WIDTH = 70;
	int count = 0;
	for (int i = 0; i < data.rows * data.cols; ++i) {
		f << data.data[0][i] << " ";
		f << data.data[1][i] << " ";
		f << data.data[2][i] << " ";
		f << "\t";
		++count;
		if (count == MAX_WIDTH) {
			f << "\n";
			count = 0;
		}
	}
	return true;
}

ImageData PpmIoStrategy::Read(const std::string& file_name) const {
	int n_rows, n_cols, max_val, red, blue, green;
	std::ifstream infile(file_name);
	std::stringstream ss;
	std::string input_line = "";
	// First line
	getline(infile, input_line);
	// Second line comment
	getline(infile, input_line);

	ss << infile.rdbuf();
	ss >> n_rows >> n_cols;
	ss >> max_val;
	ImageData image_data;
	image_data.rows = n_rows;
	image_data.cols = n_cols;
	image_data.max_val = max_val;
	std::vector<int> red_vect;
	std::vector<int> green_vect;
	std::vector<int> blue_vect;
	for (int i = 0; i < n_rows * n_cols; ++i)
	{
		ss >> red;
		ss >> green;
		ss >> blue;
		red_vect.push_back(red);
		green_vect.push_back(green);
		blue_vect.push_back(blue);
	}
	image_data.data = {red_vect, green_vect, blue_vect};
	return image_data;
}

}  // namespace igg

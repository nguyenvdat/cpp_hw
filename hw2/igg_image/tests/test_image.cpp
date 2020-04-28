#include <iostream>
#include <gtest/gtest.h>

#include "igg_image/image.h"
#include <numeric>

TEST(TestImage, ConstructorTest) {
	igg::Image im = igg::Image(4, 2);
	EXPECT_EQ(4, im.rows());
}

TEST(TestImage, AtFunctionTest) {
	igg::Image im = igg::Image(4, 2);
	im.at(0, 1) = 5;
	im.at(1, 1) = 25;

	EXPECT_EQ(5, im.at(0, 1));
	EXPECT_EQ(25, im.at(1, 1));
}

TEST(TestImage, WriteFunctionTest) {
	igg::Image im = igg::Image(4, 2);
	im.at(0, 1) = 5;
	im.at(1, 1) = 25;
	bool success = im.WriteToPgm("test.ascii.pgm");
	EXPECT_EQ(true, success);
	igg::Image im_filled = igg::Image();
	im_filled.FillFromPgm("test.ascii.pgm");
	EXPECT_EQ(5, im_filled.at(0, 1));
	EXPECT_EQ(25, im_filled.at(1, 1));
}

TEST(TestImage, FillFunctionTest) {
	igg::Image im = igg::Image();
	im.FillFromPgm("../data/lena.ascii.pgm");
	EXPECT_EQ(512, im.rows());
	EXPECT_EQ(512, im.cols());
}

TEST(TestImage, ComputeHistogramTest) {
	igg::Image im = igg::Image();
	im.FillFromPgm("../data/lena.ascii.pgm");
	auto bin_count = im.ComputeHistogram(5);
	float sum = std::accumulate(bin_count.begin(), bin_count.end(), 0.0);
	EXPECT_EQ(5, bin_count.size());
	EXPECT_EQ(1, sum);
}

TEST(TestImage, DownScaleTest) {
	igg::Image im1 = igg::Image();
	im1.FillFromPgm("../data/lena.ascii.pgm");
	igg::Image im2 = igg::Image();
	im2.FillFromPgm("../data/lena.ascii.pgm");
	im1.DownScale(2);
	EXPECT_EQ(256, im1.rows());
	EXPECT_EQ(256, im1.cols());
	EXPECT_EQ(im1.at(0, 1), im2.at(0, 2));
	EXPECT_EQ(im1.at(1, 2), im2.at(2, 4));
}

TEST(TestImage, UpScaleTest) {
	igg::Image im = igg::Image(3, 3);
	im.at(0, 0) = 1;
	im.at(0, 1) = 2;
	im.at(0, 2) = 3;
	im.at(1, 0) = 4;
	im.at(1, 1) = 5;
	im.at(1, 2) = 6;
	im.at(2, 0) = 7;
	im.at(2, 1) = 8;
	im.at(2, 2) = 9;
	// for (int i = 0; i < im.rows(); ++i)
	// {
	// 	for (int j = 0; j < im.cols(); ++j)
	// 	{
	// 		std::cout << im.at(i, j) << " ";
	// 	}
	// 	std::cout << std::endl;
	// }
	im.UpScale(3);
	EXPECT_EQ(9, im.rows());
	EXPECT_EQ(9, im.cols());
	
	im.DownScale(3);
	EXPECT_EQ(3, im.rows());
	EXPECT_EQ(3, im.cols());
	EXPECT_EQ(3, im.at(0, 2));
	EXPECT_EQ(6, im.at(1, 2));
}

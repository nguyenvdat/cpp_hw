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

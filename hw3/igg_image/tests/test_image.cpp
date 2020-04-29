#include <iostream>
#include <gtest/gtest.h>

#include "igg_image/image.h"
#include "igg_image/io_strategies/dummy_strategy.h"
#include <numeric>

TEST(TestImage, ConstructorTest) {
	igg::DummyIoStrategy dummy_strategy;
	igg::Image im1 = igg::Image(dummy_strategy);
	igg::Image im2 = igg::Image(2, 3, dummy_strategy);
}

TEST(TestImage, GetterFunctionTest) {
	igg::DummyIoStrategy dummy_strategy;
	igg::Image im = igg::Image(2, 5, dummy_strategy);
	EXPECT_EQ(2, im.rows());
	EXPECT_EQ(5, im.cols());

	igg::Image::Pixel pixel{3, 5, 6};
	im.at(1, 2) = pixel;
	EXPECT_EQ(3, im.at(1,2).red);
	EXPECT_EQ(5, im.at(1,2).green);
	EXPECT_EQ(6, im.at(1,2).blue);
}

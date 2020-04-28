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


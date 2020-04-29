#include <iostream>
#include <gtest/gtest.h>

#include "igg_image/image.h"
#include "igg_image/io_strategies/dummy_strategy.h"
#include "igg_image/io_strategies/png_strategy.h"
#include "igg_image/io_strategies/ppm_strategy.h"
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

TEST(TestImage, DownScaleTest) {
	igg::DummyIoStrategy dummy_strategy;
	igg::Image im = igg::Image(2, 5, dummy_strategy);
	im.DownScale(2);
	EXPECT_EQ(1, im.rows());
	EXPECT_EQ(3, im.cols());
}

TEST(TestImage, UpScaleTest) {
	igg::DummyIoStrategy dummy_strategy;
	igg::Image im = igg::Image(2, 5, dummy_strategy);
	im.UpScale(2);
	EXPECT_EQ(4, im.rows());
	EXPECT_EQ(10, im.cols());
}

TEST(TestImage, ReadFromDiskPngTest) {
	igg::PngIoStrategy png_strategy;
	igg::Image im = igg::Image(2, 5, png_strategy);
	im.ReadFromDisk("../data/ct1n0g04.png");
	// im.ReadFromDisk("../data/pbmlib.ascii.ppm");
}

TEST(TestImage, WriteToDiskPngTest) {
	igg::PngIoStrategy png_strategy;
	igg::Image im = igg::Image(2, 5, png_strategy);
	im.WriteToDisk("test_write_png");
}

TEST(TestImage, ReadFromDiskPpmTest) {
	igg::PpmIoStrategy ppm_strategy;
	igg::Image im = igg::Image(ppm_strategy);
	im.ReadFromDisk("../data/pbmlib.ascii.ppm");
	EXPECT_EQ(300, im.rows());
	EXPECT_EQ(300, im.cols());
	EXPECT_EQ(255, im.at(0,0).red);
	EXPECT_EQ(150, im.at(0,0).green);
	EXPECT_EQ(250, im.at(0,0).blue);
}

TEST(TestImage, WriteToDiskPpmTest) {
	igg::PpmIoStrategy ppm_strategy;
	igg::Image im = igg::Image(ppm_strategy);
	im.ReadFromDisk("../data/pbmlib.ascii.ppm");
	im.WriteToDisk("../data/test_ppm.ascii.ppmj");
	igg::Image im2 = igg::Image(ppm_strategy);
	im2.ReadFromDisk("../data/pbmlib.ascii.ppm");
	EXPECT_EQ(300, im2.rows());
	EXPECT_EQ(300, im2.cols());
	EXPECT_EQ(255, im2.at(0,0).red);
	EXPECT_EQ(150, im2.at(0,0).green);
	EXPECT_EQ(250, im2.at(0,0).blue);
}
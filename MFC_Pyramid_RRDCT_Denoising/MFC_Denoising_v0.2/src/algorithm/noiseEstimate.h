// noiseEstimate.h
// @brief: noise level estimate using PCA
// autjor cheng peng
// date  2016/8/27

#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

class NoiseEstimate {
private:
	Mat src;
	float noiseLevel;

public:
	NoiseEstimate(Mat src_) : src(src_){};
	float getNoiseLevel();
};
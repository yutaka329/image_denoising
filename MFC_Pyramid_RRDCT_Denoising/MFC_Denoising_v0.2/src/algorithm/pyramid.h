/**
  @author   Cheng Peng
  @date     2016/7/20
**/
#pragma once
#include <vector>
#include <stdlib.h>

#include "rrdct.h"

using namespace std;
using namespace cv;

class PyramidDenoising {
private:
	int pyramid_layer_num;
	vector<float> v_sigma;    //threshold of all the pyramid layer
	vector<Size> v_patch_size;  //patch size of all the pyramid layer
	vector<int> v_d;           //distance of all the pyramid layer
	//vector<Mat> gaussian;
public:
	PyramidDenoising(int layer_num, vector<float> v_sigma, vector<Size> v_patch_size, vector<int> v_d);
	void getGaussianPyramid(const Mat &src, vector<Mat> &gaussian);
	void getLaplacianPyramid(const Mat &src, vector<Mat> &gaussian, vector<Mat> &laplacian);
	void operator()(const Mat &src, Mat &dst, RandomizedRedundantDCT::SAMPLING samplingType, RandomizedRedundantDCT::BASIS basis);
	Mat addTwoMat(Mat src1, float alpha, Mat src2, float beta);
};
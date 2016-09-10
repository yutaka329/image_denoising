/**
  @author   Cheng Peng
  @date     2016/7/20
**/
#include "pyramid.h"
#include <iostream>

/*
	@brief: Constructor
*/
PyramidDenoising::PyramidDenoising(int layer_num, vector<float> v_sigma, vector<Size> v_patch_size, vector<int> v_d){
	if(layer_num >=2) {
		pyramid_layer_num = layer_num;
	}
	else {
		cerr << "Input layer_num >=2!";
	}
	this->v_sigma = v_sigma;
	this->v_patch_size = v_patch_size;
	this->v_d = v_d;
}

/*
	@brief: get Gaussian pyramid images
*/
void PyramidDenoising::getGaussianPyramid(const Mat &src, vector<Mat> &gaussian) {
	Mat tmp = src;
	gaussian.push_back(src);   //G0
	for(int i=1; i<pyramid_layer_num; i++) {
		Mat gaussian_tmp;
		pyrDown(tmp, gaussian_tmp, Size((int)tmp.cols/2, (int)tmp.rows/2));
		gaussian.push_back(gaussian_tmp);
		tmp = gaussian_tmp;
	}
}

/*
	@brief: get Laplacian pyramid images
*/
void PyramidDenoising::getLaplacianPyramid(const Mat &src, vector<Mat> &gaussian, vector<Mat> &laplacian) {
	Mat expend_gaussian;
	Mat tmp;
	vector<Mat> reverse_laplacian;
	getGaussianPyramid(src, gaussian);
	for(int i=pyramid_layer_num-2; i>=0; i--) {
		tmp = gaussian[i+1];
		pyrUp(tmp, expend_gaussian, Size(tmp.cols*2, tmp.rows*2));

		Mat laplacian_tmp;
		laplacian_tmp = addTwoMat(gaussian[i], 1.0, expend_gaussian, -1.0);
		reverse_laplacian.push_back(laplacian_tmp);
		// save images
		string filename = "../L_"+to_string(i)+".jpg";
		//imwrite(filename, laplacian_tmp);
	}

	for(int i=reverse_laplacian.size()-1; i>=0; i--) {
		laplacian.push_back(reverse_laplacian.at(i));
	}
}

/*
	@brief: Multi-scale denoising
*/
void PyramidDenoising::operator()(const Mat &src, Mat &dst, RandomizedRedundantDCT::SAMPLING samplingType, RandomizedRedundantDCT::BASIS basis) {
	Mat tmp_denoised;
	Mat expend_denoised;
	Mat tmp_detailed;
	vector<Mat> gaussian;
	vector<Mat> laplacian;	

	//PyramidDenoising pd(4, v_sigma, v_patch_size, v_d);
	//pd.getLaplacianPyramid(src, gaussian, laplacian);
	getLaplacianPyramid(src, gaussian, laplacian);

	RandomizedRedundantDCT rrdct;
	tmp_denoised = gaussian[pyramid_layer_num-1];
	for(int i=pyramid_layer_num-1; i>0; i--) {
		pyrUp(tmp_denoised, expend_denoised, Size(tmp_denoised.cols*2, tmp_denoised.rows*2));
		tmp_detailed=addTwoMat(expend_denoised, 1, laplacian[i-1], 1);
		
		rrdct.generateSamplingMaps(tmp_detailed.size(), v_patch_size[i-1], 1, v_d[i-1], samplingType);
		rrdct(tmp_detailed, tmp_denoised, v_sigma[i-1] / 10.f, v_patch_size[i-1], basis);
						
//		string filename = "../D_"+to_string(i-1)+".jpg";
//		imwrite(filename, tmp_denoised);

		string filename = "../E_"+to_string(i-1)+".jpg";
//		imwrite(filename, tmp_detailed);
	}
	dst = tmp_denoised;
}


/*
	@brief: blend two images of the same size at any weights
	@param:
	@return: the destination image.
*/
Mat PyramidDenoising::addTwoMat(Mat src1, float alpha, Mat src2, float beta) {
	if(src1.size() != src2.size()) {
		cerr << "Input images donot match!";
		exit(0);
	}
	
	Mat dst;
	dst.create(src1.size(), src1.type() );
	const int channels_1 = src1.channels();
	const int channels_2 = src2.channels();
	if(channels_1 == channels_2)  {
		const int channels = channels_1;
		switch(channels) {
		case 1:
			{
				for(int i=0; i<src1.rows;++i) {
					for(int j=0; j<src1.cols; ++j) {
						dst.at<short>(i,j) = src1.at<short>(i,j)*alpha + src2.at<short>(i,j)*beta;
					}
				}
				break;
			}
		case 3:
			{
				Mat_<Vec3s> _dst = dst;
				Mat_<Vec3s> _src1 = src1;
				Mat_<Vec3s> _src2 = src2;
				for(int i=0; i<src1.rows; ++i) {
					for(int j=0; j<src1.cols; ++j) {
						_dst(i,j)[0] = saturate_cast<short>(_src1(i,j)[0]*alpha + _src2(i,j)[0]*beta);
						_dst(i,j)[1] = saturate_cast<short>(_src1(i,j)[1]*alpha + _src2(i,j)[1]*beta);
						_dst(i,j)[2] = saturate_cast<short>(_src1(i,j)[2]*alpha + _src2(i,j)[2]*beta);
					//	_dst(i,j)[0] = (_src1(i,j)[0]*alpha + _src2(i,j)[0]*beta);
					//	_dst(i,j)[1] = (_src1(i,j)[1]*alpha + _src2(i,j)[1]*beta);
					//	_dst(i,j)[2] = (_src1(i,j)[2]*alpha + _src2(i,j)[2]*beta);
					}
				}  
				dst = _dst;
				break;
			}
		}
	}
	else {
		cerr<<"The two images don't match and cannot be added!";
	}

	return dst;
}


/*
Mat PyramidDenoising::addTwoMat(Mat src1, float alpha, Mat src2, float beta) {
	if(src1.size() != src2.size()) {
		cerr << "Input images donot match!";
		exit(0);
	}

	Mat src1_;
	if (src1.depth() != CV_32F)src1.convertTo(src1_, CV_MAKETYPE(CV_32F, src1.channels()));
	else src1_ = src1;
	Mat src2_;
	if (src2.depth() != CV_32F)src2.convertTo(src2_, CV_MAKETYPE(CV_32F, src2.channels()));
	else src2_ = src2;
	
	Mat dst;
	dst.create(src1_.size(), src1_.type() );
	const int channels_1 = src1_.channels();
	const int channels_2 = src2_.channels();
	if(channels_1 == channels_2)  {
		const int channels = channels_1;
		switch(channels) {
		case 1:
			{
				for(int i=0; i<src1_.rows;++i) {
					for(int j=0; j<src1_.cols; ++j) {
						dst.at<float>(i,j) = src1_.at<float>(i,j)*alpha + src2_.at<float>(i,j)*beta;
					}
				}
				break;
			}
		case 3:
			{
				Mat_<Vec3f> _dst = dst;
				Mat_<Vec3f> _src1 = src1_;
				Mat_<Vec3f> _src2 = src2_;
				for(int i=0; i<src1_.rows; ++i) {
					for(int j=0; j<src1_.cols; ++j) {
						//_dst(i,j)[0] = saturate_cast<float>(_src1(i,j)[0]*alpha + _src2(i,j)[0]*beta);
						//_dst(i,j)[1] = saturate_cast<float>(_src1(i,j)[1]*alpha + _src2(i,j)[1]*beta);
						//_dst(i,j)[2] = saturate_cast<float>(_src1(i,j)[2]*alpha + _src2(i,j)[2]*beta);
						_dst(i,j)[0] = (_src1(i,j)[0]*alpha + _src2(i,j)[0]*beta);
						_dst(i,j)[1] = (_src1(i,j)[1]*alpha + _src2(i,j)[1]*beta);
						_dst(i,j)[2] = (_src1(i,j)[2]*alpha + _src2(i,j)[2]*beta);
					}
				}  
				dst = _dst;
				break;
			}
		}
	}
	else {
		cerr<<"The two images don't match and cannot be added!";
	}

	return dst;
}
*/
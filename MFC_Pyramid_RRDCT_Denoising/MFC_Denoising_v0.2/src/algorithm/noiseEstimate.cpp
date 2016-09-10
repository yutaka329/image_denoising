// noiseEstimate.cpp
// @brief: noise level estimate using PCA
// autjor cheng peng
// date  2016/8/27

#include "noiseEstimate.h"

float NoiseEstimate::getNoiseLevel() {
	Mat half_src;
	pyrDown(src, half_src, Size((int)src.cols/2, (int)src.rows/2));
	Mat quarter_half;
	pyrDown(half_src, quarter_half, Size((int)half_src.cols/2, (int)half_src.rows/2));
	Mat okta_src;
	pyrDown(quarter_half, okta_src, Size((int)quarter_half.cols/2, (int)quarter_half.rows/2));

	int width = okta_src.size().width;
	int height = okta_src.size().height;
	int d = 8;   //patch size 16*16
	int channel = okta_src.channels();
	int patchNumbers = (width - d +1) * (height - d +1);
	int elementNumbers = d*d*channel;
	Mat sampleMatrixMat(patchNumbers, elementNumbers, CV_32FC1);
	
	//get sample patches
	for(int i=0; i<(height-d+1); i++) {
		for(int j=0; j<(width-d+1); j++) {
			for(int k=0; k<elementNumbers; k++) {
				int index_row = i*(width - d +1)+j;
				int index_colnum = k;
				sampleMatrixMat.at<float>(index_row, index_colnum) = (float)okta_src.at<uchar>(i+(k/(d*channel)), j*channel+(k%(d*channel))) ;	
			}
		}
	}

	IplImage *sampleMatrix = cvCloneImage(&(IplImage) sampleMatrixMat);
	
	//PCA
	Mat meanValueMat(1, elementNumbers, CV_32FC1);
	IplImage *meanValue = cvCloneImage(&(IplImage) meanValueMat);
	Mat eigenValueMat(elementNumbers, 1, CV_32FC1);
	IplImage *eigenValue = cvCloneImage(&(IplImage) eigenValueMat);
	Mat eigenValueMatrixMat(elementNumbers, elementNumbers, CV_32FC1);
	IplImage *eigenVectorMatrix = cvCloneImage(&(IplImage) eigenValueMatrixMat);
	cvCalcPCA(sampleMatrix, meanValue, eigenValue, eigenVectorMatrix,CV_PCA_DATA_AS_ROW);

	//variance estimation
	float* ptr = (float*)( eigenValue->imageData);
	float theta = 0.0;
	for(int i=0; i<eigenValue->height; i++) {
		float mean = 0;
		for(int j=i; j<eigenValue->height; j++) {
			mean += ptr[j];
		}
		mean = mean / (eigenValue->height - i + 1);
		int middle_index = (i+eigenValue->height)/2;
		if(abs(mean - ptr[middle_index]) < 10e-2) {
			theta = sqrt(mean);
			break;
		}
	}

	noiseLevel = theta;

	//release
	cvReleaseImage(&sampleMatrix);
	cvReleaseImage(&meanValue);
	cvReleaseImage(&eigenValue);
	cvReleaseImage(&eigenVectorMatrix);
	return noiseLevel;
}
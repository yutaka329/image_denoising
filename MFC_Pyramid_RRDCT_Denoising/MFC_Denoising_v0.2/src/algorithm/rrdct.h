#pragma once
#include <iostream>
#include <deque>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

enum
{
	TIME_AUTO = 0,
	TIME_NSEC,
	TIME_MSEC,
	TIME_SEC,
	TIME_MIN,
	TIME_HOUR,
	TIME_DAY
};
class CalcTime
{
	int64 pre;
	std::string mes;

	int timeMode;

	double cTime;
	bool _isShow;

	int autoMode;
	int autoTimeMode();
	std::vector<std::string> lap_mes;
public:

	void start();
	void setMode(int mode);
	void setMessage(std::string src);
	void restart();
	double getTime();
	void show();
	void show(std::string message);
	void lap(std::string message);
	void init(std::string message, int mode, bool isShow);

	CalcTime(std::string message, int mode = TIME_AUTO, bool isShow = true);
	CalcTime();

	~CalcTime();
};

class RandomizedRedundantDCT {
public:
	enum BASIS {
		DCT = 0
	};
	enum SAMPLING {
		FULL = 0,
		LATTICE,
		POISSONDISK,
	};
	bool isSSE;

	void init(cv::Size size_, int color_, cv::Size patch_size_);
	RandomizedRedundantDCT();
	RandomizedRedundantDCT(cv::Size size, int color, cv::Size patch_size_ = cv::Size(8, 8));
	

	void generateSamplingMaps(cv::Size imageSize, cv::Size patch_size, int number_of_LUT, int d, SAMPLING sampleType = SAMPLING::POISSONDISK);

	virtual void operator()(cv::Mat& src_, cv::Mat& dest, float sigma, cv::Size psize = cv::Size(8, 8), BASIS transform_basis = BASIS::DCT);
	void colorredundunt(cv::Mat& src_, cv::Mat& dest, float sigma, cv::Size psize = cv::Size(8, 8), BASIS transform_basis = BASIS::DCT);
	cv::RNG rng;

protected:
	float getThreshold(float sigmaNoise);
	BASIS basis;
	cv::Size patch_size;
	cv::Size size;
	cv::Mat buff;
	cv::Mat sum;

	cv::Mat im;

	int channel;

	void div(float* inplace0, float* inplace1, float* inplace2, float* count, const int size1);
	void div(float* inplace0, float* inplace1, float* inplace2, float* inplace3, float* count, const int size1);

	virtual void body(float *src, float* dest, float Th);

	void getSamplingFromLUT(cv::Mat& samplingMap);
	void setSamplingMap(cv::Mat& samplingMap, SAMPLING samplingType, int d);

	std::vector<cv::Mat> samplingMapLUTs;
	cv::Mat samplingMap;
};
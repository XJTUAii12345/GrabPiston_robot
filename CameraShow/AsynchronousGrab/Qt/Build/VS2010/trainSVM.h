#pragma once
#include<opencv\cv.h>
#include<opencv2\calib3d\calib3d.hpp>
#include<opencv2\features2d\features2d.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\nonfree\nonfree.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\ml\ml.hpp>
#include<algorithm>
#include<fstream>
#include<iostream>
#include<stdio.h>
using namespace std;
using namespace cv;

class trainSVM
{
public:
	trainSVM(void);
	~trainSVM(void);
	void RemoveEmptyBowImageDescriptor();	//去除无法获取Bow的图像
	void AutoTrain();					//自动网格训练
	bool ReadSvmClassifier();			//读取SvmClassifier
	bool WriteSvmClassifier();		//写SvmClasiifier
	void GetDataALabels();			//获取训练数据和标签
	CvSVMParams GetSvmParams();		//获取训练结果的参数
	void SetGridParams();			//设置网格参数
	Mat	SvmPredict(vector<Mat>& testData, vector<string> testPresents);	//预测函数
	float	SvmPredict(Mat&testData);	//预测函数
	void SetOutput(std::string& file); //设置输出xml的名称
	void SetDataALabelsVectors(vector<cv::Mat>&InputBowImagesVecotr, vector<string>& present_vector);//设置输入的数据集和标签
private:
	vector<cv::Mat>	bowImageDescriptors;
	vector<string> presents;
	CvSVMParams svmParam;
	CvSVM		svm;
	CvParamGrid CvParamGrid_C;
	CvParamGrid CvParamGrid_gamma;
	Mat        trainData;
	Mat        responses;
	string		filename;
};


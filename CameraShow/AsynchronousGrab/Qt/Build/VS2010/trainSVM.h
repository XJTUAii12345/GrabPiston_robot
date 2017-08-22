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
	void RemoveEmptyBowImageDescriptor();	//ȥ���޷���ȡBow��ͼ��
	void AutoTrain();					//�Զ�����ѵ��
	bool ReadSvmClassifier();			//��ȡSvmClassifier
	bool WriteSvmClassifier();		//дSvmClasiifier
	void GetDataALabels();			//��ȡѵ�����ݺͱ�ǩ
	CvSVMParams GetSvmParams();		//��ȡѵ������Ĳ���
	void SetGridParams();			//�����������
	Mat	SvmPredict(vector<Mat>& testData, vector<string> testPresents);	//Ԥ�⺯��
	float	SvmPredict(Mat&testData);	//Ԥ�⺯��
	void SetOutput(std::string& file); //�������xml������
	void SetDataALabelsVectors(vector<cv::Mat>&InputBowImagesVecotr, vector<string>& present_vector);//������������ݼ��ͱ�ǩ
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


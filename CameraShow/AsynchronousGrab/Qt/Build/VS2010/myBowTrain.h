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

class myBowTrain
{
public:
	myBowTrain(void);
	~myBowTrain(void);
	void SetInputWindowName(const string& InputName);
	void SetOutputWindowName(const string& OutputName);
	void SetImagesPath(const string& ImagePath);				//设置文件路径
	void GetImageVector();
	void SetFeatureDector();
	void SetDescriptorExtractor();
	void SetDescriptorMatchor(const string& DescriptorMatchor);
	void SetBowExtracotr();
	bool WriteVocabulary();
	bool ReadVocabulary();
	void SetVocabularyPath(const string& FilePath);						//设置vocabulary文件的路径及名称
	Mat TrainVocabulary();
	Mat vocabulary;
	string	Img_Name;		
	void GetImage();			//从Img_Name图片路径获取Image图像用于检测
	void SetImage(Mat& input_src);
	void ProcessImage();
	void ShowImage(Mat& Input_Img, Mat& Output_Img);
	string FeatureName;								//设定检测特征名称	
	void SetVocabularyBow();							//为bow特征抽取器设定字典
	Mat CalculateBowImage();							//计算输入图像的BowImage
	void CalculateBowImage(vector<Mat>& BowImages_vector);//计算输入图像序列的BowImage,并对对应文件进行存储
	bool WriteBowImageDescriptor(const string& file, const Mat& bowImageDescriptor); //存储BowImage图像
	bool ReadBowImageDescriptor(const string& file,  Mat& bowImageDescriptor);	//读取BowImage图像
	void OutputImagePresent(vector<std::string>& image_present);				//读取图像的标志
private:
	Mat Image;	//输入图像
	Mat ImageProcess;	//处理图像
	string vocabulary_path;	
	vector<string> img_vecotr;
	vector<string> img_present;
	string input_Window;
	string output_Window;
	Ptr<FeatureDetector> featDetector;	//特征检测器
	Ptr<DescriptorExtractor> descExtractor;	//特征提取器
	Ptr<BOWImgDescriptorExtractor>	bowExtractor;	//bow特征提取器
	Ptr<DescriptorMatcher> desMatchor;		//特征匹配器
	string images_Path;		//图像序列路径txt文件
};


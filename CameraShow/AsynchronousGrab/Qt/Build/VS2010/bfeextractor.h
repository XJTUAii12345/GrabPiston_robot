#ifndef BFEEXTRACTOR_H
#define BFEEXTRACTOR_H

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

#include <QObject>
#include <QImage>


//////////////////////////////////////////////////////////////////////////
//使用方法
//先调用preprocess()
//setVocabulary()设置字典路径
//ReadVocabulary()读取字典
//SetVocabulary()设定词袋读取器

//setSVMPath（）设定SVM分类器
//GetTest() 获取测试图像
//CalculateBowImage() 获取Bow特征
//SVMPredict（bowImageTest)
//

//////////////////////////////////////////////////////////////////////////
//或使用默认模式
//setBowExtractor
//setVocabulary()设置字典路径
//ReadVocabulary()读取字典
//SetVocabulary()设定词袋读取器

//SetSVMClassifier（）设定SVM分类器
//
//CalculateBowImage() 获取Bow特征
//SVMPredict（bowImageTest)
//////////////////////////////////////////////////////////////////////////

class BFeExtractor : public QObject
{

public:
	BFeExtractor(QObject *parent);
	BFeExtractor();
	~BFeExtractor();
	/*void SetInputWindowName(const std::string& InputName);
	void SetOutputWindowName(const std::string& OutputName);*/
	//void SetImagesPath(const std::string& ImagePath);				//设置文件路径
	//void GetImageVector();
	void SetFeatureDector();
	void SetDescriptorExtractor();
	void SetDescriptorMatchor(const std::string& DescriptorMatchor);
	void SetBowExtracotr();
	//bool WriteVocabulary();
	void SetVocabularyPath(const std::string& FilePath);		
	//读取Vocabulary
	bool ReadVocabulary();
	void SetVocabularyBow();							//为bow特征抽取器设定字典//设置vocabulary文件的路径及名称
	//cv::Mat TrainVocabulary();
	cv::Mat vocabulary;
	//std::string	Img_Name;		
	//void GetImage();			//从Img_Name图片路径获取Image图像用于检测
	void PreProcess();
	//void ShowImage(cv::Mat& Input_Img, cv::Mat& Output_Img);
	std::string FeatureName;								//设定检测特征名称	
	cv::Mat CalculateBowImage();							//计算输入图像的BowImage
	//void CalculateBowImage(std::vector<cv::Mat>& BowImages_vector); //计算输入图像序列的BowImage,并对对应文件进行存储
	//bool WriteBowImageDescriptor(const std::string& file, const cv::Mat& bowImageDescriptor); //存储BowImage图像
	//bool ReadBowImageDescriptor(const std::string& file,  cv::Mat& bowImageDescriptor);	//读取BowImage图像
	void OutputImagePresent(std::vector<std::string>& image_present);				//获取图像的标志
	
	//set svm path
	void SetSVMClassifier(std::string classifier_name);
	bool ReadSvmClassifier();			//读取SvmClassifier
	//////////////////////////////////////////////////////////////////////////
	///进行分类
	//parameters:
	//[in]	:		src
	//[out]:int result ： 【0】表示lie
	//							   【1】表示up
	//							   【2】表示down
	int GetBowCategory(cv::Mat& src);

private:
	cv::Mat Image;	//处理图像
	std::string vocabulary_path;	
	std::vector<std::string> img_vecotr;
	std::vector<std::string> img_present;
	cv::Ptr<cv::FeatureDetector> featDetector;	//特征检测器
	cv::Ptr<cv::DescriptorExtractor> descExtractor;	//特征提取器
	cv::Ptr<cv::BOWImgDescriptorExtractor>	bowExtractor;	//bow特征提取器
	cv::Ptr<cv::DescriptorMatcher> desMatchor;		//特征匹配器
	std::string images_Path;		//图像序列路径txt文件
	

	//SVM
	CvSVMParams svmParam;
	CvSVM		svm;
	CvParamGrid CvParamGrid_C;
	CvParamGrid CvParamGrid_gamma;
	cv::Mat        trainData;
	cv::Mat        responses;
	std::string		filename;
	std::string		svmFileName;
	//functions
	CvSVMParams GetSvmParams();		//获取训练结果的参数
	float	SvmPredict(cv::Mat&testData);	//预测函数
};

#endif // BFEEXTRACTOR_H

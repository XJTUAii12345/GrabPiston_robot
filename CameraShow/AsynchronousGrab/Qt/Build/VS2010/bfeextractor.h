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
//ʹ�÷���
//�ȵ���preprocess()
//setVocabulary()�����ֵ�·��
//ReadVocabulary()��ȡ�ֵ�
//SetVocabulary()�趨�ʴ���ȡ��

//setSVMPath�����趨SVM������
//GetTest() ��ȡ����ͼ��
//CalculateBowImage() ��ȡBow����
//SVMPredict��bowImageTest)
//

//////////////////////////////////////////////////////////////////////////
//��ʹ��Ĭ��ģʽ
//setBowExtractor
//setVocabulary()�����ֵ�·��
//ReadVocabulary()��ȡ�ֵ�
//SetVocabulary()�趨�ʴ���ȡ��

//SetSVMClassifier�����趨SVM������
//
//CalculateBowImage() ��ȡBow����
//SVMPredict��bowImageTest)
//////////////////////////////////////////////////////////////////////////

class BFeExtractor : public QObject
{

public:
	BFeExtractor(QObject *parent);
	BFeExtractor();
	~BFeExtractor();
	/*void SetInputWindowName(const std::string& InputName);
	void SetOutputWindowName(const std::string& OutputName);*/
	//void SetImagesPath(const std::string& ImagePath);				//�����ļ�·��
	//void GetImageVector();
	void SetFeatureDector();
	void SetDescriptorExtractor();
	void SetDescriptorMatchor(const std::string& DescriptorMatchor);
	void SetBowExtracotr();
	//bool WriteVocabulary();
	void SetVocabularyPath(const std::string& FilePath);		
	//��ȡVocabulary
	bool ReadVocabulary();
	void SetVocabularyBow();							//Ϊbow������ȡ���趨�ֵ�//����vocabulary�ļ���·��������
	//cv::Mat TrainVocabulary();
	cv::Mat vocabulary;
	//std::string	Img_Name;		
	//void GetImage();			//��Img_NameͼƬ·����ȡImageͼ�����ڼ��
	void PreProcess();
	//void ShowImage(cv::Mat& Input_Img, cv::Mat& Output_Img);
	std::string FeatureName;								//�趨�����������	
	cv::Mat CalculateBowImage();							//��������ͼ���BowImage
	//void CalculateBowImage(std::vector<cv::Mat>& BowImages_vector); //��������ͼ�����е�BowImage,���Զ�Ӧ�ļ����д洢
	//bool WriteBowImageDescriptor(const std::string& file, const cv::Mat& bowImageDescriptor); //�洢BowImageͼ��
	//bool ReadBowImageDescriptor(const std::string& file,  cv::Mat& bowImageDescriptor);	//��ȡBowImageͼ��
	void OutputImagePresent(std::vector<std::string>& image_present);				//��ȡͼ��ı�־
	
	//set svm path
	void SetSVMClassifier(std::string classifier_name);
	bool ReadSvmClassifier();			//��ȡSvmClassifier
	//////////////////////////////////////////////////////////////////////////
	///���з���
	//parameters:
	//[in]	:		src
	//[out]:int result �� ��0����ʾlie
	//							   ��1����ʾup
	//							   ��2����ʾdown
	int GetBowCategory(cv::Mat& src);

private:
	cv::Mat Image;	//����ͼ��
	std::string vocabulary_path;	
	std::vector<std::string> img_vecotr;
	std::vector<std::string> img_present;
	cv::Ptr<cv::FeatureDetector> featDetector;	//���������
	cv::Ptr<cv::DescriptorExtractor> descExtractor;	//������ȡ��
	cv::Ptr<cv::BOWImgDescriptorExtractor>	bowExtractor;	//bow������ȡ��
	cv::Ptr<cv::DescriptorMatcher> desMatchor;		//����ƥ����
	std::string images_Path;		//ͼ������·��txt�ļ�
	

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
	CvSVMParams GetSvmParams();		//��ȡѵ������Ĳ���
	float	SvmPredict(cv::Mat&testData);	//Ԥ�⺯��
};

#endif // BFEEXTRACTOR_H

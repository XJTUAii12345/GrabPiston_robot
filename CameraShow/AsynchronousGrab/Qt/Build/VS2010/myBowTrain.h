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
	void SetImagesPath(const string& ImagePath);				//�����ļ�·��
	void GetImageVector();
	void SetFeatureDector();
	void SetDescriptorExtractor();
	void SetDescriptorMatchor(const string& DescriptorMatchor);
	void SetBowExtracotr();
	bool WriteVocabulary();
	bool ReadVocabulary();
	void SetVocabularyPath(const string& FilePath);						//����vocabulary�ļ���·��������
	Mat TrainVocabulary();
	Mat vocabulary;
	string	Img_Name;		
	void GetImage();			//��Img_NameͼƬ·����ȡImageͼ�����ڼ��
	void SetImage(Mat& input_src);
	void ProcessImage();
	void ShowImage(Mat& Input_Img, Mat& Output_Img);
	string FeatureName;								//�趨�����������	
	void SetVocabularyBow();							//Ϊbow������ȡ���趨�ֵ�
	Mat CalculateBowImage();							//��������ͼ���BowImage
	void CalculateBowImage(vector<Mat>& BowImages_vector);//��������ͼ�����е�BowImage,���Զ�Ӧ�ļ����д洢
	bool WriteBowImageDescriptor(const string& file, const Mat& bowImageDescriptor); //�洢BowImageͼ��
	bool ReadBowImageDescriptor(const string& file,  Mat& bowImageDescriptor);	//��ȡBowImageͼ��
	void OutputImagePresent(vector<std::string>& image_present);				//��ȡͼ��ı�־
private:
	Mat Image;	//����ͼ��
	Mat ImageProcess;	//����ͼ��
	string vocabulary_path;	
	vector<string> img_vecotr;
	vector<string> img_present;
	string input_Window;
	string output_Window;
	Ptr<FeatureDetector> featDetector;	//���������
	Ptr<DescriptorExtractor> descExtractor;	//������ȡ��
	Ptr<BOWImgDescriptorExtractor>	bowExtractor;	//bow������ȡ��
	Ptr<DescriptorMatcher> desMatchor;		//����ƥ����
	string images_Path;		//ͼ������·��txt�ļ�
};


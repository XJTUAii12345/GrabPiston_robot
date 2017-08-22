#ifndef IMAGECVPROCESS_H
#define IMAGECVPROCESS_H
enum Piston_categroy{ Lie=0, Up=1,Down =2};


#include <QObject>
#include <QImage>
#include <opencv.hpp>
#include <opencv/highgui.h>
#include <cv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

#include "bfeextractor.h"

#include "trainSVM.h"
#include "myBowTrain.h"



class ImageCVProcess : public QObject
{
private: 
	QImage frame_qimage;
	QImage q_current_resultImage;
	cv::Mat image_mat;
	//ROI 区域
	cv::Mat roi_1;
	cv::Mat roi_2;
	cv::Mat roi_3;
	cv::Mat roi_4;
	//ROI 分类
	Piston_categroy roi_cate_1;
	Piston_categroy roi_cate_2;
	Piston_categroy roi_cate_3;
	Piston_categroy roi_cate_4;

	//Bow分类器
	BFeExtractor bow_feature_extractor;
	///私有化函数

	// Prints out a given logging string, error code and the descriptive representation of that error code
	//
	// Parameters:
	//  [in]    strMsg          A given message to be printed out
	//  [in]    eErr            The API status code
	//
	void Dilation( int dilation_elem,int dilation_size, cv::Mat &src,cv:: Mat & dilation_dst );

	// Prints out a given logging string, error code and the descriptive representation of that error code
	//
	// Parameters:
	//  [in]    strMsg          A given message to be printed out
	//  [in]    eErr            The API status code
	//
	cv::Point CaculateRectCenterLine(cv::Point2f* _rotateRect);

	// Prints out a given logging string, error code and the descriptive representation of that error code
	//
	// Parameters:
	//  [in]    strMsg          A given message to be printed out
	//  [in]    eErr            The API status code
	//

	void LieROIDetect(cv::Mat& src , cv::Mat &dst, cv::Point &center_point , float &lie_rotation);

	// Prints out a given logging string, error code and the descriptive representation of that error code
	//
	// Parameters:
	//  [in]    strMsg          A given message to be printed out
	//  [in]    eErr            The API status code
	//
	void UpROIDetect(cv::Mat& src , cv::Mat &dst, cv::Point &center_point , float &lie_rotation);

	// Prints out a given logging string, error code and the descriptive representation of that error code
	//
	// Parameters:
	//  [in]    strMsg          A given message to be printed out
	//  [in]    eErr            The API status code
	//
	void DownROIDetect(cv::Mat& src , cv::Mat &dst, cv::Point &center_point , float &lie_rotation);

	// Prints out a given logging string, error code and the descriptive representation of that error code
	//
	// Parameters:
	//  [in]    strMsg          A given message to be printed out
	//  [in]    eErr            The API status code
	//
	
	//计算旋转矩阵
	cv::Mat CalculateRotationMatrix();
	cv::Mat Rotation_Matrix;

public:
	ImageCVProcess();
	ImageCVProcess(QObject *parent);
	ImageCVProcess(QImage &pframe_in);
	~ImageCVProcess();
	
	//
	void ResetImage(QImage &pframe_in);
	QImage GetSplitImage(int case_in);
	QImage CVMat2QImage(const cv::Mat& mat) ;
	float GetRotation(Piston_categroy piston_in, int index_roi);
	QPoint GetLocation(Piston_categroy piston_in, int index_roi);
	void GetLocation(Piston_categroy piston_in, int index_roi, QPoint &center_out, float &rotation_out);
	Piston_categroy DetectPistonCategory(int index_roi);
	//获取当前的处理后的结果图片
	QImage GetCurrentResultRoiImage();
	//用myBowTrain和trainSVM来进行识别
	Piston_categroy DetectPistonCategory(myBowTrain& _inputBowTrain, trainSVM& _inputSVM,int index_roi);

	//获取机器人的坐标进给距离和旋转角度
	//【in】Vec3f [0]:X pixel ; [1]: Y pixel; [2]: Rotation
	//【out】Vec3f [0]: X mm;[1]:Y mm; [2]: A  degree
	cv::Vec3f GetRobotDestPoint(cv::Vec3f info_input);

};

#endif // IMAGECVPROCESS_H

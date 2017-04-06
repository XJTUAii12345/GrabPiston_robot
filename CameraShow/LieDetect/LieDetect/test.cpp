#include <opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include <iostream>

void Dilation( int dilation_elem,int dilation_size, cv::Mat &src,cv:: Mat & dilation_dst );
cv::Point caculateRectCenterLine(cv::Point2f* _rotateRect);
int main(int agrc , char** argv)
{
	cv::Mat img = cv::imread("50_3.jpg");
	cv::imshow("img",img);
	cv::waitKey(100);
	cv::Mat img_gray;
	cv::cvtColor(img,img_gray,cv::COLOR_BGR2GRAY);
	cv::Mat cdst=img.clone();

	//imgprocess
	cv::GaussianBlur(img_gray,img_gray,cv::Size(3,3),0);
	cv::threshold(img_gray,img_gray,180,220,CV_THRESH_BINARY);
	cv::Canny(img_gray,img_gray,50,80,3);//50,80,3
	Dilation(0,3,img_gray.clone(),img_gray);
	cv::imshow( "Dilation Demo", img_gray );
	cv::imshow("image_gray",img_gray);
	//contours
	cv::vector<cv::vector<cv::Point> > contours;
	cv::findContours( img_gray.clone(), contours,	CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
	
	//查找最大轮廓  
	double maxarea = 0;  
	int maxAreaIdx = 0;  
	for (int index = contours.size() - 1; index >= 0; index--)  
	{  
		double tmparea = fabs(contourArea(contours[index]));  
		if (tmparea>maxarea)  
		{  
			maxarea = tmparea;  
			maxAreaIdx = index;//记录最大轮廓的索引号  
		}  
	}  
	cv::vector<cv::Point> max_contourlist;
	max_contourlist = contours[maxAreaIdx];  

	// draw each connected component with its own random color
	std::cout<<"print the contours number is  "<<contours.size()<<"\n 最大轮廓Index为"<<maxAreaIdx<<std::endl;
	
	cv::vector<cv::vector<cv::Point>> maxContour;
	maxContour.push_back(max_contourlist);
	cv::Scalar color( 200,120,75);
	cv::drawContours(cdst,maxContour,0,color,CV_FILLED);
	//cv::drawContours( cdst, contours.max_size(),0, color,2 , 8 );//CV_FILLED
	cv::namedWindow( "Contours", 1 );
	cv::imshow( "Contours",cdst );

	//arclength,获取角度
	double peri = cv::arcLength(max_contourlist,true);

	//获取最大拟合多边形
	cv::vector<cv::Point> cnt_approx;
	cv::approxPolyDP(max_contourlist,cnt_approx,0.002*peri,true);
	
	//利用最大拟合多边形，拟合近似角度曲线
	cv::RotatedRect cnt_rect = cv::minAreaRect(cnt_approx);
	cv::Point2f cnt_vertices[4];
	cnt_rect.points(cnt_vertices);
	for (int i = 0; i < 4; i++)
		cv::line(cdst, cnt_vertices[i], cnt_vertices[(i+1)%4], cv::Scalar(0,0,255));
	cv::Rect brect = cnt_rect.boundingRect();
	cv::rectangle(cdst, brect,cv:: Scalar(255,0,0));
	std::cout<<"旋转角度为：	"<<cnt_rect.angle<<std::endl;

	//获取拟合多边形的质心位置
	cv::Point short_line = caculateRectCenterLine(cnt_vertices);
	
	cv::line(cdst,short_line,cnt_rect.center,cv::Scalar(0,0,255),2,8);
	circle( cdst, cnt_rect.center, 3, cv::Scalar(0,255,0), 3, 8, 0 );

	//计算旋转角度
	float Rotation=0;
	cv::Point RectCenter =cnt_rect.center;
	float Y = (RectCenter.y-short_line.y);
	float X = (RectCenter.x-short_line.x);
	Rotation = atan2(Y,X);
	float Angle = Rotation * 180/CV_PI;
	std::cout<<"旋转角度为	"<<Angle<<std::endl;
	cv::imshow("边框",cdst);
	cv::waitKey();
	return 1;
}

void Dilation( int dilation_elem,int dilation_size, cv::Mat &src,cv:: Mat & dilation_dst )
{
	int dilation_type;
	if( dilation_elem == 0 ){ dilation_type = cv::MORPH_RECT; }
	else if( dilation_elem == 1 ){ dilation_type = cv::MORPH_CROSS; }
	else if( dilation_elem == 2) { dilation_type = cv::MORPH_ELLIPSE; }

	cv::Mat element = cv::getStructuringElement( dilation_type,
		cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
		cv::Point( dilation_size, dilation_size ) );
	///膨胀操作
	cv::dilate( src, dilation_dst, element );
}

cv::Point caculateRectCenterLine(cv::Point2f* _rotateRect)
{
	double distance_line_1=0.0;
	double distance_line_2=0.0;
	cv::Point center_point;
	distance_line_1 =  abs(_rotateRect[0].x - _rotateRect[(1)%4].x) + abs(_rotateRect[0].y - _rotateRect[(1)%4].y);
	distance_line_2 =  abs(_rotateRect[1].x - _rotateRect[(2)%4].x) + abs(_rotateRect[1].y - _rotateRect[(2)%4].y);
	if (distance_line_2>distance_line_1)
	{
		center_point.x = (_rotateRect[0].x + _rotateRect[1].x)/2;
		center_point.y = (_rotateRect[0].y +_rotateRect[1].y)/2;
	} 
	else
	{
		center_point.x = (_rotateRect[2].x + _rotateRect[1].x)/2;
		center_point.y = (_rotateRect[2].y +_rotateRect[1].y)/2;
	}
	std::cout<<"short line center point is "<<center_point.x<<"	"<<center_point.y<<std::endl;
	return center_point;
}
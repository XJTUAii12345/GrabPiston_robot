#include <opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main(int arc, char** argv)
{
	cv::Mat img = cv::imread("84_1.jpg");
	cv::imshow("img",img);
	cv::waitKey(100);
	cv::Mat img_gray;
	cv::cvtColor(img,img_gray,cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(img_gray,img_gray,cv::Size(5,5),0);
	cv::threshold(img_gray,img_gray,180,220,CV_THRESH_BINARY);
	cv::Canny(img_gray,img_gray,30,80,3);//50,80,3
	cv::imshow("canny",img_gray);
	cv::Mat cdst=img.clone();
	cv::vector<cv::Vec2f> lines;
	cv::HoughLines(img_gray, lines, 1, CV_PI/180, 44, 0, 0 );//44
	std::cout<<"lines is "<<lines.size()<<std::endl;
	for( size_t i = 0; i < lines.size(); i++ )
	{
		float rho = lines[i][0], theta = lines[i][1];
		cv::Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		line( cdst, pt1, pt2, cv::Scalar(0,0,255), 3, CV_AA);
	}
	cv::imshow("detected lines", cdst);
	cv::waitKey(10);

	//houghcircles
	cv::vector<cv::Vec3f> circles;
	cv::HoughCircles(img_gray, circles, CV_HOUGH_GRADIENT,2,img_gray.rows/4,60);//2, img_gray.rows/4, 200, 100,img_gray.rows/4,img_gray.rows/3
	std::cout<<"the number of circles is  "<<circles.size()<<std::endl;
	for( size_t i = 0; i < circles.size(); i++ )
	{
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// draw the circle center
		circle( cdst, center, 3, cv::Scalar(0,255,0), 3, 8, 0 );
		// draw the circle outline
		circle( cdst, center, radius, cv::Scalar(255,0,0), 3, 8, 0 );
	}
	cv::namedWindow( "circles", 1 );
	cv::imshow( "circles", cdst );
	cv::waitKey();
	return 1;
}

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc ,char** argv)
{
	Point2f src[4];
	src[0] = Point2f(600,-600);
	src[1] = Point2f(0,-600);
	src[2] = Point2f(600,0);
	src[3] = Point2f(0,0);
	Point2f dst[4];
	dst[0] = Point2f(524.752,-573.7807);
	dst[1] = Point2f(-22.745,-541.6195);
	dst[2] = Point2f(551.8005,-32.0326);
	dst[3] = Point2f(0.0,0.0);
	Mat per_transform = getPerspectiveTransform(src,dst);
	vector<Point2f> origin_point(1);
	origin_point[0]=Point2f(300,-300);
	vector<Point2f> origin_trans(1);  
	cout<<per_transform<<endl;
	cv::perspectiveTransform(origin_point,origin_trans,per_transform);
	std::cout<<origin_trans[0]<<std::endl;

	//获取旋转矩阵
	Point2f center(0,0);
	double angle = 3; //逆时针方向旋转3度
	double scale = 300.0;
	Mat rot_Mat = getRotationMatrix2D(center,angle,scale);
	cout<<rot_Mat<<endl;
	getchar();
	return 1;
}
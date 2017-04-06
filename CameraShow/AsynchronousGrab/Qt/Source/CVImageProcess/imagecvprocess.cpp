#include "imagecvprocess.h"

bool GreaterPointSort (cv::Point2d a,cv::Point2d b) { return (a.x>b.x); } 

ImageCVProcess::ImageCVProcess(QObject *parent)
	: QObject(parent)
{
	//初始化词袋特征模型
	std::string vocabulary_path = "./vocabulary.xml.gz";
	std::string svm_path ="SVM_Surf.xml";
	bow_feature_extractor.SetBowExtracotr();
	bow_feature_extractor.SetVocabularyPath(vocabulary_path);
	bow_feature_extractor.SetVocabularyBow();
	bow_feature_extractor.SetSVMClassifier(svm_path);
	bow_feature_extractor.ReadSvmClassifier();
	CalculateRotationMatrix();
}

ImageCVProcess::ImageCVProcess()
{
	//初始化词袋特征模型
	std::string vocabulary_path = "vocabulary.xml.gz";
	std::string svm_path ="SVM_Surf.xml";

	bow_feature_extractor.FeatureName = " surf";
	bow_feature_extractor.SetFeatureDector();
	bow_feature_extractor.SetDescriptorExtractor();
	bow_feature_extractor.SetDescriptorMatchor("BruteForce");
	bow_feature_extractor.SetBowExtracotr();
	bow_feature_extractor.SetVocabularyPath(vocabulary_path);
	bow_feature_extractor.ReadVocabulary();
	bow_feature_extractor.SetVocabularyBow();
	bow_feature_extractor.SetSVMClassifier(svm_path);
	bow_feature_extractor.ReadSvmClassifier();
	CalculateRotationMatrix();
}
ImageCVProcess::ImageCVProcess( QImage &pframe_in )
{
	const int byte_per_line = pframe_in.bytesPerLine();
	int height = pframe_in.height();
	int width = pframe_in.width();
	cv::Mat iMat(height,width, CV_8UC3,(void*)pframe_in.constBits(),(int)byte_per_line);
	image_mat = iMat.clone();

	//初始化词袋特征模型
	std::string vocabulary_path = "vocabulary.xml.gz";
	std::string svm_path ="SVM_Surf.xml";
	bow_feature_extractor.SetBowExtracotr();
	bow_feature_extractor.SetVocabularyPath(vocabulary_path);
	bow_feature_extractor.SetVocabularyBow();
	bow_feature_extractor.SetSVMClassifier(svm_path);
	bow_feature_extractor.ReadSvmClassifier();
}

ImageCVProcess::~ImageCVProcess()
{

}

QImage ImageCVProcess::GetSplitImage(int case_in)
{
	cv::Rect ROI_rect ; 
	cv::Mat ROI;
	/*cv::imshow("img_show",image_mat);
	cv::waitKey(100);*/
	switch(case_in)
	{
	case  1:
		ROI_rect =cv::Rect(537,317,300 ,300);
		image_mat(ROI_rect).copyTo(ROI);
		roi_1 = ROI.clone();
		/*cv::imshow("ROI_1",ROI);*/
		break;
	case 2:
		ROI_rect =cv::Rect(837,317,300 ,300);
		image_mat(ROI_rect).copyTo(ROI);
		roi_2= ROI.clone();
		break;
	case 3:
		ROI_rect =cv::Rect(537,617,300, 300);
		image_mat(ROI_rect).copyTo(ROI);
		roi_3 = ROI.clone();
		break;
	case 4:
		ROI_rect =cv::Rect(837,617,300 , 300);
		image_mat(ROI_rect).copyTo(ROI);
		roi_4 = ROI.clone();
		break;
	default:
		ROI_rect =cv::Rect(0,0,1 , 1);
		image_mat(ROI_rect).copyTo(ROI);
		break;
	}
	return CVMat2QImage(ROI);
}

QImage ImageCVProcess::CVMat2QImage(const cv::Mat& mat)  
{  
	// 8-bits unsigned, NO. OF CHANNELS = 1  
	if(mat.type() == CV_8UC1)  
	{  
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);  
		// Set the color table (used to translate colour indexes to qRgb values)  
		image.setColorCount(256);  
		for(int i = 0; i < 256; i++)  
		{  
			image.setColor(i, qRgb(i, i, i));  
		}  
		// Copy input Mat  
		uchar *pSrc = mat.data;  
		for(int row = 0; row < mat.rows; row ++)  
		{  
			uchar *pDest = image.scanLine(row);  
			memcpy(pDest, pSrc, mat.cols);  
			pSrc += mat.step;  
		}  
		return image;  
	}  
	// 8-bits unsigned, NO. OF CHANNELS = 3  
	else if(mat.type() == CV_8UC3)  
	{  
		// Copy input Mat  
		const uchar *pSrc = (const uchar*)mat.data;  
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);  
		return image.rgbSwapped();  
	}  
	else if(mat.type() == CV_8UC4)  
	{  
//		qDebug() << "CV_8UC4";  
		// Copy input Mat  
		const uchar *pSrc = (const uchar*)mat.data;  
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);  
		return image.copy();  
	}  
	else  
	{  
	//	qDebug() << "ERROR: Mat could not be converted to QImage.";  
		return QImage();  
	}  
}  

void ImageCVProcess::Dilation( int dilation_elem,int dilation_size, cv::Mat &src,cv:: Mat & dilation_dst )
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

cv::Point ImageCVProcess::CaculateRectCenterLine( cv::Point2f* _rotateRect )
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
	// 输出短边的中心位置
	std::cout<<"short line center point is "<<center_point.x<<"	"<<center_point.y<<std::endl;
	return center_point;
}

void ImageCVProcess::LieROIDetect( cv::Mat& src , cv::Mat &dst, cv::Point &center_point , float &lie_rotation )
{
		cv::imshow("src",src);
		cv::waitKey(100);
		cv::Mat img_gray;
		cv::cvtColor(src,img_gray,cv::COLOR_BGR2GRAY);
		cv::Mat cdst=src.clone();

		//imgprocess
		cv::GaussianBlur(img_gray,img_gray,cv::Size(3,3),0);
		cv::threshold(img_gray,img_gray,180,220,CV_THRESH_BINARY);
		cv::Canny(img_gray,img_gray,50,80,3);//50,80,3
		Dilation(0,3,img_gray.clone(),img_gray);
		cv::imshow( "Dilation Demo", img_gray );

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
		//绘制最大轮廓
		cv::vector<cv::vector<cv::Point>> maxContour;
		maxContour.push_back(max_contourlist);
		cv::Scalar color( 200,120,75);
		cv::drawContours(cdst,maxContour,0,color,CV_FILLED);
		cv::namedWindow( "Contours", 1 );
		cv::imshow( "Contours",cdst );

		//拟合最大多边形
		double peri = cv::arcLength(max_contourlist,true);
		cv::vector<cv::Point> cnt_approx;
		cv::approxPolyDP(max_contourlist,cnt_approx,0.002*peri,true);
		//利用最大拟合多边形，拟合近似矩形
		cv::RotatedRect cnt_rect = cv::minAreaRect(cnt_approx);
		cv::Point2f cnt_vertices[4];
		cnt_rect.points(cnt_vertices);
			///绘制旋转多边形
		for (int i = 0; i < 4; i++)
			cv::line(cdst, cnt_vertices[i], cnt_vertices[(i+1)%4], cv::Scalar(0,0,255));
			///绘制最大多边形
		cv::Rect brect = cnt_rect.boundingRect();
		cv::rectangle(cdst, brect,cv:: Scalar(255,0,0));
		std::cout<<"旋转角度为：	"<<cnt_rect.angle<<std::endl;

		//获取近似矩形质心
		cv::Point RectCenter =cnt_rect.center;
		circle( cdst, RectCenter, 3, cv::Scalar(0,255,0), 3, 8, 0 );

		//获取拟合多边形的质心位置
		cv::Point short_line = CaculateRectCenterLine(cnt_vertices);
		cv::line(cdst,short_line,cnt_rect.center,cv::Scalar(0,0,255),2,8);

		//计算旋转角度
		float Rotation=0;
		float Y = (RectCenter.y-short_line.y);
		float X = (RectCenter.x-short_line.x);
		Rotation = atan2(Y,X);
		float Angle = Rotation * 180/CV_PI;

		lie_rotation = Angle;
		center_point = RectCenter;
		std::cout<<"旋转角度为	"<<Angle<<std::endl;

		cv::imshow("边框",cdst);
		cv::waitKey(100);

		q_current_resultImage  = CVMat2QImage(cdst);

		dst = cdst.clone();
}

void ImageCVProcess::UpROIDetect( cv::Mat& src , cv::Mat &dst, cv::Point &center_point , float &lie_rotation )
{
	cv::Mat img_gray;
	cv::cvtColor(src,img_gray,cv::COLOR_BGR2GRAY);
	cv::Mat cdst=src.clone();
	//灰度图像处理
	cv::GaussianBlur(img_gray,img_gray,cv::Size(7,7),0);
	cv::threshold(img_gray,img_gray,160,255,CV_THRESH_BINARY);
	cv::imshow("threshold2",img_gray);
	cv::Canny(img_gray,img_gray,80,200,3);//50,80,3
	Dilation(1,1,img_gray.clone(),img_gray);
	cv::imshow("canny",img_gray);
	//
	//cv::vector<cv::Vec2f> lines;
	//cv::HoughLines(img_gray, lines, 1, CV_PI/180, 70, 0, 0 );//44
	//std::cout<<"lines is "<<lines.size()<<std::endl;

	//float theta_all=0.0;
	//for( size_t i = 0; i < lines.size(); i++ )
	//{
	//	float rho = lines[i][0], theta = lines[i][1];
	//	theta_all += theta;
	//	cv::Point pt1, pt2;
	//	double a = cos(theta), b = sin(theta);
	//	double x0 = a*rho, y0 = b*rho;
	//	pt1.x = cvRound(x0 + 1000*(-b));
	//	pt1.y = cvRound(y0 + 1000*(a));
	//	pt2.x = cvRound(x0 - 1000*(-b));
	//	pt2.y = cvRound(y0 - 1000*(a));
	//	line( cdst, pt1, pt2, cv::Scalar(0,0,255), 3, CV_AA);
	//}
	//cv::imshow("detected lines", cdst);
	//cv::waitKey(10);
	//计算第二大联通区域
	//contours
	cv::vector<cv::vector<cv::Point> > contours;
	cv::findContours( img_gray.clone(), contours,	CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
	//查找轮廓  
	vector<Point2d> contours_area;
	for (int index = contours.size() - 1; index >= 0; index--)  
	{  
		double tmparea = fabs(contourArea(contours[index]));  
		contours_area.push_back(Point2d(tmparea,index));
	}  
	//轮廓大小排序
	std::sort(contours_area.begin(),contours_area.end(),GreaterPointSort);
	int sec_max_Index = contours_area[1].y;
	cv::vector<cv::Point> sec_max_contourlist;
	sec_max_contourlist = contours[sec_max_Index];  
	//绘制轮廓
	cv::vector<cv::vector<cv::Point>> secmaxContour;
	secmaxContour.push_back(sec_max_contourlist);
	cv::Scalar color( 200,120,75);
	cv::drawContours(cdst,secmaxContour,0,color,CV_FILLED);
	cv::namedWindow( "Contours", 1 );
	cv::imshow( "Contours",cdst );

	//拟合最大多边形
	double peri = cv::arcLength(sec_max_contourlist,true);
	cv::vector<cv::Point> cnt_approx;
	cv::approxPolyDP(sec_max_contourlist,cnt_approx,0.002*peri,true);
	//利用最大拟合多边形，拟合近似矩形
	cv::RotatedRect cnt_rect = cv::minAreaRect(cnt_approx);
	cv::Point2f cnt_vertices[4];
	cnt_rect.points(cnt_vertices);
	///绘制旋转多边形
	for (int i = 0; i < 4; i++)
		cv::line(cdst, cnt_vertices[i], cnt_vertices[(i+1)%4], cv::Scalar(0,0,255));
	///绘制最大多边形
	cv::Rect brect = cnt_rect.boundingRect();
	cv::rectangle(cdst, brect,cv:: Scalar(255,0,0));
	//拟合多边形质心
	cv::Point RectCenter =cnt_rect.center;

	//获取拟合多边形的质心位置
	cv::Point short_line = CaculateRectCenterLine(cnt_vertices);
	cv::line(cdst,short_line,cnt_rect.center,cv::Scalar(0,0,255),2,8);

	//计算旋转角度
	float Rotation=0;
	float Y = (RectCenter.y-short_line.y);
	float X = (RectCenter.x-short_line.x);
	Rotation = atan2(Y,X);
	float Angle = Rotation * 180/CV_PI;

	lie_rotation = Angle;
	std::cout<<"旋转角度为	"<<Angle<<std::endl;


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
		circle( cdst, center, radius, cv::Scalar(255,0,0), 2, 8, 0 );
	}
	
	center_point = cv::Point(cvRound(circles[0][0]), cvRound(circles[0][1]));

	////统计平均旋转角度
	//float theta_avg = theta_all/lines.size();
	//float t = (theta_avg*180.0)/CV_PI;
	//lie_rotation= 90.0 - theta_avg*180.0/CV_PI;
	//设定输出图像
	dst=cdst.clone();
	q_current_resultImage  = CVMat2QImage(cdst);
	//显示输出图像
	cv::namedWindow( "circles", 1 );
	cv::imshow( "circles", dst );
	cv::waitKey(100);
}

Piston_categroy ImageCVProcess::DetectPistonCategory( int index_roi)
{
	cv::Mat roi_process;
	switch(index_roi)
	{
	case  1:
		roi_process = roi_1;
		break;
	case 2:
		roi_process = roi_2;
		break;
	case 3:
		roi_process = roi_3;
		break;
	case 4:
		roi_process = roi_4;
		break;
	default:
		std::cerr<<"error of not right index\n";
		throw "处理ROI的索引位置存在问题";
		break;
	}
	int result=bow_feature_extractor.GetBowCategory(roi_process);
	return Piston_categroy(result);
}

Piston_categroy ImageCVProcess::DetectPistonCategory( myBowTrain& _inputBowTrain, trainSVM& _inputSVM ,int index_roi)
{
	cv::Mat roi_process;
	switch(index_roi)
	{
	case  1:
		roi_process = roi_1;
		cv::imshow("roi",roi_1);
		break;
	case 2:
		roi_process = roi_2;
		break;
	case 3:
		roi_process = roi_3;
		break;
	case 4:
		roi_process = roi_4;
		break;
	default:
		std::cerr<<"error of not right index\n";
		throw "处理ROI的索引位置存在问题";
		break;
	}
	
	_inputBowTrain.SetImage(roi_process);
	Mat bowTestImage;
	bowTestImage = _inputBowTrain.CalculateBowImage();
	float result=-1.0;
	result = _inputSVM.SvmPredict(bowTestImage);
	return Piston_categroy((int)result);
}

QPoint ImageCVProcess::GetLocation( Piston_categroy piston_in, int index_roi)
{
	cv::Mat roi_process;
	switch(index_roi)
	{
	case  1:
		roi_process = roi_1;
		break;
	case 2:
		roi_process = roi_2;
		break;
	case 3:
		roi_process = roi_3;
		break;
	case 4:
		roi_process = roi_4;
		break;
	default:
		 std::cerr<<"error of not right index\n";
		 throw "处理ROI的索引位置存在问题";
		break;
	}
	cv::Mat roi_result;
	cv::Point pistion_center;
	float piston_rotation=0.0f;

	switch(piston_in)
	{
	case Up:
		UpROIDetect(roi_process,roi_result,pistion_center, piston_rotation);
		break;
	case Lie:
		LieROIDetect(roi_process,roi_result,pistion_center, piston_rotation);
		break;
	case Down:
		UpROIDetect(roi_process,roi_result,pistion_center, piston_rotation);
		break;
	default:
		 throw "处理Piston 分类的索引存在问题";
		break;
	}
	QPoint current_point ;
	current_point.setX(pistion_center.x);
	current_point.setY(pistion_center.y);
	return current_point;
}

void ImageCVProcess::GetLocation( Piston_categroy piston_in, int index_roi, QPoint &center_out, float &rotation_out )
{
	cv::Mat roi_process;
	cv::Point2f current_roi_point;
	switch(index_roi)
	{
	case  1:
		roi_process = roi_1;
		current_roi_point = cv::Point2f(537,317);
		break;
	case 2:
		roi_process = roi_2;
		current_roi_point = cv::Point2f(837,317);
		break;
	case 3:
		roi_process = roi_3;
		current_roi_point = cv::Point2f(537,617);
		break;
	case 4:
		roi_process = roi_4;
		current_roi_point = cv::Point2f(837,617);
		break;
	default:
		std::cerr<<"error of not right index\n";
		throw "处理ROI的索引位置存在问题";
		break;
	}
	cv::Mat roi_result;
	cv::Point pistion_center;
	float piston_rotation=0.0f;

	switch(piston_in)
	{
	case Up:
		UpROIDetect(roi_process,roi_result,pistion_center, piston_rotation);
		break;
	case Lie:
		LieROIDetect(roi_process,roi_result,pistion_center, piston_rotation);
		break;
	case Down:
		DownROIDetect(roi_process,roi_result,pistion_center, piston_rotation);
		break;
	default:
		throw "处理Piston 分类的索引存在问题";
		break;
	}
	center_out.setX(pistion_center.x+current_roi_point.x);
	center_out.setY(pistion_center.y+current_roi_point.y);
	rotation_out = piston_rotation;
}

float ImageCVProcess::GetRotation( Piston_categroy piston_in , int index_roi)
{
	return 0.0f;
}

void ImageCVProcess::ResetImage(QImage &pframe_in)
{
	const int byte_per_line = pframe_in.bytesPerLine();
	int height = pframe_in.height();
	int width = pframe_in.width();
	cv::Mat iMat(height,width, CV_8UC3,(void*)pframe_in.constBits(),(int)byte_per_line);
	image_mat = iMat.clone();
}

QImage ImageCVProcess::GetCurrentResultRoiImage()
{
	return q_current_resultImage;
}

void ImageCVProcess::DownROIDetect( cv::Mat& src , cv::Mat &dst, cv::Point &center_point , float &lie_rotation )
{
	cv::Mat img_gray;
	cv::cvtColor(src,img_gray,cv::COLOR_BGR2GRAY);
	cv::Mat cdst=src.clone();
	//灰度图像处理
	cv::GaussianBlur(img_gray,img_gray,cv::Size(5,5),0);
	cv::threshold(img_gray,img_gray,180,220,CV_THRESH_BINARY);
	cv::Canny(img_gray,img_gray,30,80,3);//50,80,3
	cv::imshow("canny",img_gray);

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
		circle( cdst, center, radius, cv::Scalar(255,0,0), 2, 8, 0 );
	}
	//设定返回值
	center_point = cv::Point(cvRound(circles[0][0]), cvRound(circles[0][1]));
	lie_rotation = 0;
	
	//设定输出图像
	dst=cdst.clone();
	q_current_resultImage  = CVMat2QImage(cdst);
	//显示输出图像
	cv::namedWindow( "circles", 1 );
	cv::imshow( "circles", dst );
	cv::waitKey(100);
}

cv::Mat ImageCVProcess::CalculateRotationMatrix()
{
	cv::Point2f src[4];
	src[0] =cv::Point2f(600,-600);
	src[1] = cv::Point2f(0,-600);
	src[2] = cv::Point2f(600,0);
	src[3] =cv:: Point2f(0,0);
	cv::Point2f dst[4];
	dst[0] = cv::Point2f(524.752,-573.7807);
	dst[1] = cv::Point2f(-22.745,-541.6195);
	dst[2] = cv::Point2f(551.8005,-32.0326);
	dst[3] =cv:: Point2f(0.0,0.0);
	cv::Mat per_transform = getPerspectiveTransform(src,dst);
	std::cout<<per_transform<<std::endl;
	Rotation_Matrix = per_transform.clone();
	return per_transform;
}

cv::Vec3f ImageCVProcess::GetRobotDestPoint( cv::Vec3f info_input )
{
	//获取图像像素点相对于右下角点的对应坐标

	//获取右下角点对应机器人的坐标
	cv::vector<Point2f> origin_point(1);
	//Transform图像坐标映射
	origin_point[0]=cv::Point2f(info_input[0],info_input[1]);
	origin_point[0] = origin_point[0]-cv::Point2f(1137,917);
	origin_point[0].x=0-origin_point[0].x;
	//origin_point[0]=cv::Point2f(300.0,-300.0);
	cv::vector<Point2f> origin_trans(1);  
	float  robot_angle = info_input[2];
	cv::perspectiveTransform(origin_point,origin_trans,Rotation_Matrix);
	std::cout<<origin_trans[0]<<std::endl;
	cv::Vec3f robot_info;
	robot_info = cv::Vec3f(origin_trans[0].x,origin_trans[0].y,robot_angle);
	return robot_info;
}



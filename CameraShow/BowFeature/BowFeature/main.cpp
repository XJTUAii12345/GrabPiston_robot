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

class myTrain
{
public:
	myTrain();
	~myTrain();
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

myTrain::myTrain()
{
	featDetector->create("SURF");
	descExtractor->create("SURF");
	desMatchor->create("BruteForce");
}

myTrain::~myTrain()
{
}

void myTrain::SetInputWindowName(const string& InputName)
{
	input_Window = InputName;
}

void myTrain::SetOutputWindowName(const string& OutputName)
{
	output_Window = OutputName;
}

void myTrain::SetImagesPath(const string& ImagePath)
{
	images_Path.clear();
	images_Path = ImagePath;
}
//获取图像序列的图像地址向量，和图像标记
void myTrain::GetImageVector()
{
	img_vecotr.clear();
	img_present.clear();
	std::fstream gtfile(images_Path.c_str());
	if (!gtfile.is_open())
	{
		string err_mgs = "could not open the image file '" + images_Path + "'.";
		CV_Error(CV_StsError, err_mgs.c_str());
	}
	string line;
	string imagename;
	string present;
	cout << "extracting images data and present……" << endl;
	while (!gtfile.eof())
	{
		std::getline(gtfile, line);
		std::istringstream iss(line);//将gtfile提取的Line与其中的字符绑定
		iss >> imagename >> present;
		if (!iss.fail())
		{
			img_vecotr.push_back(imagename);
			img_present.push_back(present);
		}
		else
		{
			if (!gtfile.eof())
			{
				CV_Error(CV_StsParseError, "error in parsing image vector txt file");
			}
		}
	}
}
//输出图像的标志
void myTrain::OutputImagePresent(vector<std::string>& image_present)
{
	image_present.clear();
	image_present = img_present;
}

void myTrain::SetFeatureDector()
{
	featDetector=FeatureDetector::create(FeatureName);
}
void myTrain::SetDescriptorExtractor()
{
	descExtractor=DescriptorExtractor::create(FeatureName);
}
void myTrain::SetDescriptorMatchor(const string& DescriptorMatchor)
{
	desMatchor =DescriptorMatcher::create(DescriptorMatchor);
}
void myTrain::SetBowExtracotr()
{
	bowExtractor = new BOWImgDescriptorExtractor(descExtractor, desMatchor);
}
Mat myTrain::TrainVocabulary()
{
	if (img_vecotr.empty())
	{
		CV_Error(CV_StsError, "Error in the image vector is empty. Please use GetImageVector before.");
	}
	if (featDetector.empty() || descExtractor.empty() || desMatchor.empty())
	{
		CV_Error(CV_StsError, "Error in the image FeatureDetector and DescriptorExtractor or DescriptorMatchor is empty");
	}
	//bowExtractor = new BOWImgDescriptorExtractor(descExtractor, desMatchor);
	CV_Assert(descExtractor->descriptorType() == CV_32FC1);
	cout << "computing images descriptors" << endl;
	RNG& rng = theRNG();
	TermCriteria term_criterition;
	term_criterition.epsilon = FLT_EPSILON;
	BOWKMeansTrainer bowTrainer(500, term_criterition, 3, KMEANS_PP_CENTERS);
	while (img_vecotr.size()>0)
	{	
		//随机抽取图像向量中的图像进行特征提取
		int randImgIdx = rng((unsigned)img_vecotr.size());
		Mat Image = imread(img_vecotr[randImgIdx]);
		vector<KeyPoint> Keypoints;
		featDetector->detect(Image, Keypoints);
		Mat DescriptorMat;
		descExtractor->compute(Image, Keypoints, DescriptorMat);
		if (!DescriptorMat.empty())
		{
			int descCount = DescriptorMat.rows;//特征描述矩阵中特征的数目
			for (int i = 0; i < descCount; i++)
			{
				bowTrainer.add(DescriptorMat.row(i));
			}
		}
		img_vecotr.erase(img_vecotr.begin() + randImgIdx);
	}
	cout << "actual descriptor count of the bow trainer" << bowTrainer.descripotorsCount() << endl;
	cout << "Training vocabulary..." << endl;
	vocabulary = bowTrainer.cluster();
	return vocabulary;
}

void myTrain::SetVocabularyPath(const string& FilePath)
{
	vocabulary_path = string(FilePath);
}

bool myTrain::WriteVocabulary()
{
	cout << "Saving vocabulary..." << endl;
	FileStorage fs(vocabulary_path, FileStorage::WRITE);
	if (fs.isOpened())
	{
		fs << "vocabulary" << vocabulary;
		return true;
	}
	return false;
}

bool myTrain::ReadVocabulary()
{
	vocabulary.release();
	cout << "Reading vocabulary..." << endl;
	FileStorage fr(vocabulary_path, FileStorage::READ);
	if (fr.isOpened())
	{
		fr["vocabulary"] >> vocabulary;
		cout << "done" << endl;
		return true;
	}
	return false;
}

/*单个图像处理程序应用*/
void myTrain::ProcessImage()
{
	if (Img_Name.empty())
	{
		CV_Error(CV_StsError, "Error in the Single Input Image Name");
	}
	Image = imread(Img_Name);
	if (!Image.empty())
	{
		/*进行特征检测并画出特征点*/
		vector<KeyPoint> keypoints;
		/*检测是否检测器和抽取器已经设置好*/
		if (featDetector.empty()||descExtractor.empty())
		{
			CV_Error(CV_StsError, "Error in the image FeatureDetector and DescriptorExtractor or DescriptorMatchor is empty");
		}
		featDetector->detect(Image, keypoints);
		Mat KeyPointMat;
		Image.copyTo(KeyPointMat);
		drawKeypoints(Image, keypoints, KeyPointMat, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		imshow("KeyPoints", KeyPointMat);
		vector<KeyPoint>::const_iterator itKeypoint = keypoints.begin();
		for (; itKeypoint != keypoints.end(); ++itKeypoint)
		{
			cout << "Position   " << itKeypoint->pt << endl;
			cout << "diameter of the meaningful keypoint neighborhood  " << itKeypoint->size << endl;
			cout << "Angle  " << itKeypoint->angle << endl;
			cout << "value  " << itKeypoint->response << endl;
			printf("octave  %d\n", itKeypoint->octave);
		}
		descExtractor->compute(Image, keypoints, ImageProcess);
		ShowImage(Image, ImageProcess);
	}
	else
	{
		CV_Error(CV_StsError, "Error the Input Image is empty");
	}
}
void myTrain::ShowImage(Mat& Input, Mat& Output)
{
	cv::namedWindow(input_Window);
	cv::namedWindow(output_Window);
	imshow(input_Window, Input);
	imshow(output_Window, Output);
}
void myTrain::GetImage()
{
	if (!Img_Name.empty())
	{
		Image = imread(Img_Name);
	}
}
/*为bow特征抽取器设定字典*/
void myTrain::SetVocabularyBow()
{
	if (vocabulary.empty())
	{
		CV_Error(CV_StsError, "Error in the vocabulary is empty.");
	}
	bowExtractor->setVocabulary(vocabulary);
}
/*计算单个图像的bow特征图像并存储为xml的特征描述矩阵*/
Mat myTrain::CalculateBowImage()
{
	if (!Image.empty())
	{
		vector<KeyPoint> keypoints;
		featDetector->detect(Image, keypoints);
		Mat BowDescriptor;
		bowExtractor->compute(Image, keypoints, BowDescriptor);
		if (!BowDescriptor.empty())
		{
			/*if (!WriteBowImageDescriptor("exmaple.xml", BowDescriptor))
			{
				cout << "Error: file emaple.xml can not be opened to write bow image descriptor" << endl;
				exit(-1);
			}*/
			return BowDescriptor;
		}
			//imshow("exmple_xml", BowDescriptor);
	}
}
/*计算矩阵序列的Bowt特征序列*/
void myTrain::CalculateBowImage(vector<Mat>& BowImages_vector)
{
	BowImages_vector.clear();
	GetImageVector();
	if (!img_vecotr.empty())
	{
		if (featDetector.empty() || descExtractor.empty() || desMatchor.empty())
		{
			CV_Error(CV_StsError, "Error in the image FeatureDetector and DescriptorExtractor or DescriptorMatchor is empty");
		}
		CV_Assert(descExtractor->descriptorType() == CV_32FC1);
		cout << "computing images Bowdescriptors" << endl;
			//imshow("exmple_xml", BowDescriptor);
		for (vector<string>::const_iterator it = img_vecotr.begin(); it != img_vecotr.end(); ++it)
		{
			Mat inputImage;
			Mat BowDescriptorImage;
			string filename = *it;
			filename.replace(filename.find(".jpg"), 4, ".xml");
			if (ReadBowImageDescriptor(filename, BowDescriptorImage))
			{
				cout << "Loaded bag of word vector for image "  +*it+"  from the existed descriptor" << endl;
				BowImages_vector.push_back(BowDescriptorImage);
			}
			else
			{
				inputImage = imread(*it);
				vector<KeyPoint> keypoints;
				cout << "Computing descriptors for image " ;
				featDetector->detect(inputImage, keypoints);
				bowExtractor->compute(inputImage, keypoints, BowDescriptorImage);
				cout << " + generating BoW vector" << endl;
				if (!BowDescriptorImage.empty())
				{
					if (!WriteBowImageDescriptor(filename, BowDescriptorImage))
					{
						cout << "Error: file " << filename << "can not be opened to write bow image descriptor" << endl;
						exit(-1);
					}
				}
				BowImages_vector.push_back(BowDescriptorImage);
			}
		}
	}
	else
	{
		CV_Error(CV_StsError, "Error in the image vector is empty. Please check the image vector input txt file.");
	}
}
//写BowImage的描述矩阵
bool myTrain::WriteBowImageDescriptor(const string& file, const Mat& bowImageDescriptor)
{
	FileStorage fs(file, FileStorage::WRITE);
	if (fs.isOpened())
	{
		fs << "BowimageDescriptor" << bowImageDescriptor;
		return true;
	}
	return false;
}
//读取BowImage特征存储在描述矩阵中
bool myTrain::ReadBowImageDescriptor(const string& file,Mat& bowImageDescriptor)
{
	bowImageDescriptor.release();
	cout << "Reading BOWImageDescriptor..." << endl;
	FileStorage fr(file, FileStorage::READ);
	if (fr.isOpened())
	{
		fr["BowimageDescriptor"] >> bowImageDescriptor;
		cout << "done" << endl;
		return true;
	}
	return false;
}

/****************Train SVM Classifier*************************/
class trainSVM
{
public:
	trainSVM();
	~trainSVM();
	void RemoveEmptyBowImageDescriptor();	//去除无法获取Bow的图像
	void AutoTrain();					//自动网格训练
	bool ReadSvmClassifier();			//读取SvmClassifier
	bool WriteSvmClassifier();		//写SvmClasiifier
	void GetDataALabels();			//获取训练数据和标签
	CvSVMParams GetSvmParams();		//获取训练结果的参数
	void SetGridParams();			//设置网格参数
	Mat	SvmPredict(vector<Mat>& testData, vector<string> testPresents);	//预测函数
	float	SvmPredict(Mat&testData);	//预测函数
	void SetOutput(std::string& file); //设置输出xml的名称
	void SetDataALabelsVectors(vector<cv::Mat>&InputBowImagesVecotr, vector<string>& present_vector);//设置输入的数据集和标签
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

trainSVM::trainSVM()
{
}

trainSVM::~trainSVM()
{
}
 //设置输出xml的名称
void trainSVM::SetOutput(std::string& file)
{
	filename.clear();
	filename = file;
}
void trainSVM::SetDataALabelsVectors(vector<cv::Mat>&InputBowImagesVecotr, vector<string>& present_vector)
{
	bowImageDescriptors.clear();
	bowImageDescriptors = InputBowImagesVecotr;
	presents.clear();
	presents = present_vector;
}
//去除空BowImageDescriptor
void trainSVM::RemoveEmptyBowImageDescriptor()
{
	CV_Assert(!bowImageDescriptors.empty());
	for (int i = (int)bowImageDescriptors.size() - 1; i >= 0; i--)
	{
		bool res = bowImageDescriptors[i].empty();
		if (res)
		{
			cout << "Removing the ith image in training due to no descriptors..." << endl;
			bowImageDescriptors.erase(bowImageDescriptors.begin() + i);
			presents.erase(presents.begin() + i);
		}
	}
}
//获取训练数据和标签
void trainSVM::GetDataALabels()
{
	trainData.create((int)bowImageDescriptors.size(), bowImageDescriptors[0].cols, CV_32FC1);//注意trainData的矩阵大小
	responses.create((int)bowImageDescriptors.size(), 1, CV_32FC1);	
	/*设定标签矩阵*/
	//vector<Mat> Labels;
	//Labels.push_back((Mat_<float>(1, 3) << 1, -1, -1));   
	//Labels.push_back((Mat_<float>(1, 3) << -1, 1, -1));
	//Labels.push_back((Mat_<float>(1, 3) << -1, -1, 1));
	//cout << "标签矩阵：" << endl;
	//cout << Labels[0]<<endl;
	//cout << Labels[1] << endl;
	//cout << Labels[2] << endl;

	// 将训练数据矩阵填充为bag of word 向量的特征矩阵，同时填充结果矩阵
	for (size_t imageIdx = 0; imageIdx < bowImageDescriptors.size(); imageIdx++)
	{
		// Transfer image descriptor (bag of words vector) to training data matrix
		Mat submat = trainData.row((int)imageIdx);
		bowImageDescriptors[imageIdx].copyTo(submat);

		// Set response value
		//responese标记负样本为-1，正样本为1
		/*Mat subret = responses.row((int)imageIdx);
		int labret = atoi(presents[imageIdx].c_str());
		Labels[labret].copyTo(subret);*/
		responses.at<float>(imageIdx, 0) = (float)atoi(presents[imageIdx].c_str());
	}
	cout << responses<<endl;
	cout << "TRAINING SVM FOR CLASS ..." << endl;
}
//读取SVM
bool trainSVM::ReadSvmClassifier()
{	
	FileStorage fs(filename, FileStorage::READ);
	if (fs.isOpened())
	{
		cout << "*** LOADING SVM CLASSIFIER FOR CLASS "  << endl;
		svm.load(filename.c_str());
		return true;
	}
	else
	{
		return false;
	}
}
//写SVM
bool trainSVM::WriteSvmClassifier()
{
	if (!filename.empty())
	{
		svm.save(filename.c_str());
		return true;
	}
	else
	{
		return false;
	}
}
//设置网格参数
void trainSVM::SetGridParams()
{
	CvParamGrid_C=CvParamGrid(pow(2.0, -5), pow(2.0, 15), pow(2.0, 1));
	CvParamGrid_gamma = CvParamGrid(pow(2.0, -15), pow(2.0, 3), pow(2.0, -1));
}
//获取训练参数
CvSVMParams trainSVM::GetSvmParams()
{
	cout << "gamma:" << svm.get_params().gamma << endl;
	cout << "C:" << svm.get_params().C << endl;
	//cout << "save the surf_svm_xml" << endl;
	return svm.get_params();
}
//自动训练
void trainSVM::AutoTrain()
{
	if (!ReadSvmClassifier())
	{
		svmParam.kernel_type = CvSVM::LINEAR;
		svmParam.svm_type = CvSVM::C_SVC;
		svmParam.term_crit = cvTermCriteria(CV_TERMCRIT_EPS, 10000, FLT_EPSILON);
		/*paramz.C = 100;
		paramz.gamma = 8.0;*/
		//☆☆☆☆☆☆☆☆☆(5)SVM学习☆☆☆☆☆☆☆☆☆☆☆☆    
		//svm.train(trainData_mat, res_mat, Mat(), Mat(), paramz);
		svm.train_auto(trainData, responses, Mat(), Mat(), svmParam, 10, CvParamGrid_C, CvParamGrid_gamma, CvSVM::get_default_grid(CvSVM::P),
			CvSVM::get_default_grid(CvSVM::NU), CvSVM::get_default_grid(CvSVM::COEF), CvSVM::get_default_grid(CvSVM::DEGREE));//训练啦      
		//☆☆利用训练数据和确定的学习参数,进行SVM学习☆☆☆☆    
		if (WriteSvmClassifier())
		{
			cout << "save the svm classifier" << filename << endl;
		}
		else
		{
			CV_Error(CV_StsError, "Error in writing svm classifier xml file");
		}
	}
}
//进行SVM的整体预测
Mat	trainSVM::SvmPredict(vector<Mat>& testData,vector<string> testPresents)	//预测函数
{
	Mat testDat;
	Mat testRet;
	if (ReadSvmClassifier())
	{
		testDat.create((int)testData.size(), testData[0].cols, CV_32FC1);//注意trainData的矩阵大小
		testRet.create((int)testData.size(), 1, CV_32FC1);
		/*设定标签矩阵*/
		/*vector<Mat> Labels;
		Labels.push_back((Mat_<float>(1, 3) << 1, -1, -1));
		Labels.push_back((Mat_<float>(1, 3) << -1, 1, -1));
		Labels.push_back((Mat_<float>(1, 3) << -1, -1, 1));
		cout << Labels[2] << endl;*/

		// 将训练数据矩阵填充为bag of word 向量的特征矩阵，同时填充结果矩阵
		for (size_t imageIdx = 0; imageIdx < testData.size(); imageIdx++)
		{
			// Transfer image descriptor (bag of words vector) to training data matrix
			Mat submat = testDat.row((int)imageIdx);
			testData[imageIdx].copyTo(submat);

			// Set response value
			/*Mat subret = responses.row((int)imageIdx);
			int labret = atoi(presents[imageIdx].c_str());
			Labels[labret].copyTo(subret);*/
			testRet.at<float>(imageIdx, 0) = (float)atoi(testPresents[imageIdx].c_str());
		}
		cout << "TESTING SVM FOR CLASS ..." << endl;
	}
	Mat result;
	svm.predict(testDat, result);
	//**********存储结果到文件**********
	FileStorage fresult("result.xml", FileStorage::WRITE);
	if (fresult.isOpened())
	{
		fresult << "BowimageDescriptor" <<result;
	}
	//***********输出结果准确率**************
	float correct_num = 0.0;
	float accuray = 0.0;
	for (int i = 0; i < testData.size(); i++)
	{
		if((int)(result.at<float>(i, 0)) == (int)(testRet.at<float>(i, 0))) correct_num++;
	}
	accuray = correct_num / testData.size();
	cout << "accuray is:	"<<accuray << endl;
	return result;
}
//进行SVM的单个预测
float trainSVM::SvmPredict(Mat&testData)
{
		float result =-1.0;
		result = svm.predict(testData,true);
		return result;
}
int main(int argc, char** argv)
{
	initModule_nonfree();//if use SIFT or SURF
	const char* keys =
		"{ f | feature     | SURF| specify the feature wanna extract}"
		"{ i | input       | example.JPG| specify the image wanna be extracted}"
		"{ p | imgPath       |imageVector_train.txt| specify the input image vector .txt filepath}"
		"{ v | vocabulary       | vocabulary.xml.gz| specify the name of the trained vocabulary}"
		"{ s | svmfile       | SVM_Surf.xml| specify the name of the svm classifier }";
	CommandLineParser cmd(argc, argv, keys);
	string feature_name = cmd.get<string>("feature");  //获取特征名称
	string imgPath = cmd.get<string>("imgPath");		//获取图像向量文件路径
	string vocPath = cmd.get<string>("vocabulary");		//获取字典存储文件路径
	string inputImage = cmd.get<string>("input");		//获取单个输入图像的路径
	string outputSVM = cmd.get<string>("svmfile");		//获取SVMfile文件名

	//myTrain mytrain;
	//mytrain.FeatureName = feature_name;
	////设置图像序列的路径
	//mytrain.SetImagesPath(imgPath);
	//mytrain.SetFeatureDector();
	//mytrain.SetDescriptorExtractor();
	//mytrain.SetDescriptorMatchor("BruteForce");
	//mytrain.SetBowExtracotr();
	////设置vocabulary的路径
	//mytrain.SetVocabularyPath(vocPath);
	////获取图像序列向量
	//mytrain.GetImageVector();
	//if (!mytrain.ReadVocabulary())
	//{
	//	mytrain.TrainVocabulary();
	//	if (!mytrain.vocabulary.empty())
	//	{
	//		if (mytrain.WriteVocabulary())	cout << "Write vocabulary is done" << endl;
	//	}
	//	else
	//	{
	//		cout << "Error: Vocbulary is empty" << endl;
	//		exit(-1);
	//	}
	//	
	//}
	//else
	//{
	//	if (mytrain.vocabulary.empty())
	//	{
	//		cout << "Error: file " << vocPath << " can not be opened to write" << endl;
	//		exit(-1);
	//	}
	//}
	////imshow("vocabulary", mytrain.vocabulary);
	//mytrain.SetVocabularyBow();
	//mytrain.Img_Name = inputImage;
	//mytrain.GetImage();
	////mytrain.CalculateBowImage();
	////获取类内图片或图片序列的Bow特征
	//vector<Mat> BowImages;
	////由mytrain类内函数获取图像序列BowImage特征
	//mytrain.CalculateBowImage(BowImages);
	////训练SVM分类器
	trainSVM mySVM;
	mySVM.SetOutput(outputSVM);
	vector<std::string> imgPresent_vector;
	//由mytrain类内函数获取图像序列标签
//	mytrain.OutputImagePresent(imgPresent_vector);
	/*mySVM.SetDataALabelsVectors(BowImages, imgPresent_vector);
	mySVM.RemoveEmptyBowImageDescriptor();
	mySVM.GetDataALabels();
	mySVM.SetGridParams();*/
	mySVM.AutoTrain();
	mySVM.GetSvmParams();
	/*********进行预测************/
	myTrain mytest;
	mytest.SetImagesPath("imageVector_train.txt");
	mytest.SetVocabularyPath(vocPath);
	mytest.FeatureName = feature_name;
	mytest.SetFeatureDector();
	mytest.SetDescriptorExtractor();
	mytest.SetDescriptorMatchor("BruteForce");
	mytest.SetBowExtracotr();
	mytest.GetImageVector();
	if (!mytest.ReadVocabulary())
	{

			cout << "Error: Vocbulary is not inputed" << endl;
			exit(-1);
	}
	else
	{
		if (mytest.vocabulary.empty())
		{
			cout << "Error: file " << vocPath << " can not be opened to write" << endl;
			exit(-1);
		}
	}
	mytest.SetVocabularyBow();
	//获取输入测试图片的Bow特征
	/*vector<Mat> BowTestImages;
	mytest.CalculateBowImage(BowTestImages);
	vector<std::string> TestPresent_vector;
	mytest.OutputImagePresent(TestPresent_vector);
	Mat res=mySVM.SvmPredict(BowTestImages, TestPresent_vector);
	cout << "the result is *****" << endl;
	cout << res << endl;*/
	mytest.Img_Name = inputImage;
	mytest.GetImage();
	Mat bowTestImage;
	bowTestImage = mytest.CalculateBowImage();
	float result=-1.0;
	result = mySVM.SvmPredict(bowTestImage);
	cout << "print the input single image result is the class" << endl;
	cout << result << endl;
	getchar();
	return 1;
}

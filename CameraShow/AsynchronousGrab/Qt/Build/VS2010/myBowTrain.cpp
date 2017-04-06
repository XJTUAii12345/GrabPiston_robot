#include "myBowTrain.h"


myBowTrain::myBowTrain(void)
{
	cv::initModule_nonfree();
}


myBowTrain::~myBowTrain(void)
{
}

void myBowTrain::SetInputWindowName( const string& InputName )
{
	input_Window = InputName;
}

void myBowTrain::SetOutputWindowName( const string& OutputName )
{
	output_Window = OutputName;
}

void myBowTrain::SetImagesPath( const string& ImagePath )
{
	images_Path.clear();
	images_Path = ImagePath;
}

void myBowTrain::GetImageVector()
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

void myBowTrain::SetFeatureDector()
{
	featDetector=FeatureDetector::create(FeatureName);
}

void myBowTrain::SetDescriptorExtractor()
{
	descExtractor=DescriptorExtractor::create(FeatureName);
}

void myBowTrain::SetDescriptorMatchor( const string& DescriptorMatchor )
{
	desMatchor =DescriptorMatcher::create(DescriptorMatchor);
}

void myBowTrain::SetBowExtracotr()
{
	bowExtractor = new BOWImgDescriptorExtractor(descExtractor, desMatchor);
}

bool myBowTrain::WriteVocabulary()
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

bool myBowTrain::ReadVocabulary()
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

void myBowTrain::SetVocabularyPath( const string& FilePath )
{
	vocabulary_path = string(FilePath);
}

cv::Mat myBowTrain::TrainVocabulary()
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

void myBowTrain::GetImage()
{
	if (!Img_Name.empty())
	{
		Image = imread(Img_Name);
	}
}

void myBowTrain::ProcessImage()
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

void myBowTrain::ShowImage( Mat& Input_Img, Mat& Output_Img )
{
	cv::namedWindow(input_Window);
	cv::namedWindow(output_Window);
	imshow(input_Window, Input_Img);
	imshow(output_Window, Output_Img);
}

void myBowTrain::SetVocabularyBow()
{
	if (vocabulary.empty())
	{
		CV_Error(CV_StsError, "Error in the vocabulary is empty.");
	}
	bowExtractor->setVocabulary(vocabulary);
}

cv::Mat myBowTrain::CalculateBowImage()
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

void myBowTrain::CalculateBowImage( vector<Mat>& BowImages_vector )
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

bool myBowTrain::WriteBowImageDescriptor( const string& file, const Mat& bowImageDescriptor )
{
	FileStorage fs(file, FileStorage::WRITE);
	if (fs.isOpened())
	{
		fs << "BowimageDescriptor" << bowImageDescriptor;
		return true;
	}
	return false;
}

bool myBowTrain::ReadBowImageDescriptor( const string& file, Mat& bowImageDescriptor )
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

void myBowTrain::OutputImagePresent( vector<std::string>& image_present )
{
	image_present.clear();
	image_present = img_present;
}

//设定分类图像
//[in]  Mat& 输入要进行特征提取图像的指针
void myBowTrain::SetImage( Mat& input_src )
{
	Image = input_src.clone();
}





#include "bfeextractor.h"

BFeExtractor::BFeExtractor(QObject *parent)
	: QObject(parent)
{
	
}

BFeExtractor::BFeExtractor()
{
	cv::initModule_nonfree();  
}

BFeExtractor::~BFeExtractor()
{

}

void BFeExtractor::SetFeatureDector()
{
	cv::initModule_nonfree(); 
	featDetector = cv::FeatureDetector::create(FeatureName);
}

void BFeExtractor::SetDescriptorExtractor()
{
	cv::initModule_nonfree(); 
	descExtractor=cv::DescriptorExtractor::create(FeatureName);
}

void BFeExtractor::SetDescriptorMatchor( const std::string& DescriptorMatchor )
{
	desMatchor =cv::DescriptorMatcher::create(DescriptorMatchor);
}

void BFeExtractor::SetBowExtracotr()
{
		bowExtractor = new cv::BOWImgDescriptorExtractor(descExtractor, desMatchor);
}

void BFeExtractor::SetVocabularyPath( const std::string& FilePath )
{
	vocabulary_path = std::string(FilePath);
}

bool BFeExtractor::ReadVocabulary()
{
	vocabulary.release();
	cv::FileStorage fr(vocabulary_path, cv::FileStorage::READ);
	if (fr.isOpened())
	{
		fr["vocabulary"] >> vocabulary;
		return true;
	}
	else
	{
		return false;
	}
}

void BFeExtractor::SetVocabularyBow()
{
	if (vocabulary.empty())
	{
		CV_Error(CV_StsError, "Error in the vocabulary is empty.");
	}
	bowExtractor->setVocabulary(vocabulary);
}

cv::Mat BFeExtractor::CalculateBowImage()
{
		if (!Image.empty())
	{
		std::vector<cv::KeyPoint> keypoints;
		featDetector->detect(Image, keypoints);
		cv::Mat BowDescriptor;
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
	}
}

bool BFeExtractor::ReadSvmClassifier()
{
	cv::FileStorage fs(svmFileName, cv::FileStorage::READ);
	if (fs.isOpened())
	{
		std::cout << "*** LOADING SVM CLASSIFIER FOR CLASS "  << std::endl;
		svm.load(svmFileName.c_str());
		GetSvmParams();
		return true;
	}
	else
	{
		return false;
	}
}

CvSVMParams BFeExtractor::GetSvmParams()
{
	std::cout << "gamma:" << svm.get_params().gamma <<std:: endl;
	std::cout << "C:" << svm.get_params().C <<std:: endl;
	//cout << "save the surf_svm_xml" << endl;
	return svm.get_params();
}

float BFeExtractor::SvmPredict( cv::Mat&testData )
{
	float result =-1.0;
	result = svm.predict(testData,true);
	return result;
}

void BFeExtractor::PreProcess()
{
	SetFeatureDector();
	SetDescriptorExtractor();
	SetDescriptorMatchor("BruteForce");
	SetBowExtracotr();
}

void BFeExtractor::SetSVMClassifier( std::string classifier_name )
{
	svmFileName = classifier_name;
}

int BFeExtractor::GetBowCategory( cv::Mat& src )
{
	Image = src.clone();
	cv::Mat bow_mat = CalculateBowImage();
	float predict  = -1;
	predict = SvmPredict(bow_mat);
	return (int)predict;
}




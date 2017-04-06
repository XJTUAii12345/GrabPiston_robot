#include "trainSVM.h"


trainSVM::trainSVM(void)
{
}


trainSVM::~trainSVM(void)
{
}

//�������xml������
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
//ȥ����BowImageDescriptor
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
//��ȡѵ�����ݺͱ�ǩ
void trainSVM::GetDataALabels()
{
	trainData.create((int)bowImageDescriptors.size(), bowImageDescriptors[0].cols, CV_32FC1);//ע��trainData�ľ����С
	responses.create((int)bowImageDescriptors.size(), 1, CV_32FC1);	
	/*�趨��ǩ����*/
	//vector<Mat> Labels;
	//Labels.push_back((Mat_<float>(1, 3) << 1, -1, -1));   
	//Labels.push_back((Mat_<float>(1, 3) << -1, 1, -1));
	//Labels.push_back((Mat_<float>(1, 3) << -1, -1, 1));
	//cout << "��ǩ����" << endl;
	//cout << Labels[0]<<endl;
	//cout << Labels[1] << endl;
	//cout << Labels[2] << endl;

	// ��ѵ�����ݾ������Ϊbag of word ��������������ͬʱ���������
	for (size_t imageIdx = 0; imageIdx < bowImageDescriptors.size(); imageIdx++)
	{
		// Transfer image descriptor (bag of words vector) to training data matrix
		Mat submat = trainData.row((int)imageIdx);
		bowImageDescriptors[imageIdx].copyTo(submat);

		// Set response value
		//responese��Ǹ�����Ϊ-1��������Ϊ1
		/*Mat subret = responses.row((int)imageIdx);
		int labret = atoi(presents[imageIdx].c_str());
		Labels[labret].copyTo(subret);*/
		responses.at<float>(imageIdx, 0) = (float)atoi(presents[imageIdx].c_str());
	}
	cout << responses<<endl;
	cout << "TRAINING SVM FOR CLASS ..." << endl;
}
//��ȡSVM
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
//дSVM
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
//�����������
void trainSVM::SetGridParams()
{
	CvParamGrid_C=CvParamGrid(pow(2.0, -5), pow(2.0, 15), pow(2.0, 1));
	CvParamGrid_gamma = CvParamGrid(pow(2.0, -15), pow(2.0, 3), pow(2.0, -1));
}
//��ȡѵ������
CvSVMParams trainSVM::GetSvmParams()
{
	cout << "gamma:" << svm.get_params().gamma << endl;
	cout << "C:" << svm.get_params().C << endl;
	//cout << "save the surf_svm_xml" << endl;
	return svm.get_params();
}
//�Զ�ѵ��
void trainSVM::AutoTrain()
{
	if (!ReadSvmClassifier())
	{
		svmParam.kernel_type = CvSVM::LINEAR;
		svmParam.svm_type = CvSVM::C_SVC;
		svmParam.term_crit = cvTermCriteria(CV_TERMCRIT_EPS, 10000, FLT_EPSILON);
		/*paramz.C = 100;
		paramz.gamma = 8.0;*/
		//����������(5)SVMѧϰ�������������    
		//svm.train(trainData_mat, res_mat, Mat(), Mat(), paramz);
		svm.train_auto(trainData, responses, Mat(), Mat(), svmParam, 10, CvParamGrid_C, CvParamGrid_gamma, CvSVM::get_default_grid(CvSVM::P),
			CvSVM::get_default_grid(CvSVM::NU), CvSVM::get_default_grid(CvSVM::COEF), CvSVM::get_default_grid(CvSVM::DEGREE));//ѵ����      
		//�������ѵ�����ݺ�ȷ����ѧϰ����,����SVMѧϰ�����    
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
//����SVM������Ԥ��
Mat	trainSVM::SvmPredict(vector<Mat>& testData,vector<string> testPresents)	//Ԥ�⺯��
{
	Mat testDat;
	Mat testRet;
	if (ReadSvmClassifier())
	{
		testDat.create((int)testData.size(), testData[0].cols, CV_32FC1);//ע��trainData�ľ����С
		testRet.create((int)testData.size(), 1, CV_32FC1);
		/*�趨��ǩ����*/
		/*vector<Mat> Labels;
		Labels.push_back((Mat_<float>(1, 3) << 1, -1, -1));
		Labels.push_back((Mat_<float>(1, 3) << -1, 1, -1));
		Labels.push_back((Mat_<float>(1, 3) << -1, -1, 1));
		cout << Labels[2] << endl;*/

		// ��ѵ�����ݾ������Ϊbag of word ��������������ͬʱ���������
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
	//**********�洢������ļ�**********
	FileStorage fresult("result.xml", FileStorage::WRITE);
	if (fresult.isOpened())
	{
		fresult << "BowimageDescriptor" <<result;
	}
	//***********������׼ȷ��**************
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
//����SVM�ĵ���Ԥ��
float trainSVM::SvmPredict(Mat&testData)
{
		float result =-1.0;
		result = svm.predict(testData,true);
		return result;
}
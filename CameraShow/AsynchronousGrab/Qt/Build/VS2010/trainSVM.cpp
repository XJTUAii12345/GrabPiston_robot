#include "trainSVM.h"


trainSVM::trainSVM(void)
{
}


trainSVM::~trainSVM(void)
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
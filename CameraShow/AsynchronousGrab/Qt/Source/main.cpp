/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        main.cpp

  Description: The main entry point of the AsynchronousGrab example of VimbaCPP.

-------------------------------------------------------------------------------

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF TITLE,
  NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR  PURPOSE ARE
  DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/

#include "AsynchronousGrab.h"
#include <opencv.hpp>
#include <QApplication>
#include<opencv2\features2d\features2d.hpp>
#include "trainSVM.h"
#include "myBowTrain.h"

int main( int argc, char *argv[] )
{
    QApplication a( argc, argv );
    AsynchronousGrab w;
	cv::initModule_nonfree();
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

	////训练SVM分类器
	trainSVM mySVM;
	mySVM.SetOutput(outputSVM);
	vector<std::string> imgPresent_vector;
	//由mytrain类内函数获取图像序列标签
	mySVM.AutoTrain();
	mySVM.GetSvmParams();
	/*********进行预测************/
	myBowTrain mytest;
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
	mytest.Img_Name = inputImage;
	mytest.GetImage();
	Mat bowTestImage;
	bowTestImage = mytest.CalculateBowImage();
	float result=-1.0;
	result = mySVM.SvmPredict(bowTestImage);
	cout << "print the input single image result is the class" << endl;
	cout << result << endl;
	//利用mytest和mySVM来进行预测
	w.InitialTrainedSVM();
	w.InitialTrainedBow();
    w.show();
    return a.exec();
}

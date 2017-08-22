/*=============================================================================
  Copyright (C) 2012 - 2016 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        AsynchronousGrab.cpp

  Description: Qt dialog class for the GUI of the AsynchronousGrab example of
               VimbaCPP.

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

#include <sstream>

#include "AsynchronousGrab.h"
#include "VmbTransform.h"
#define NUM_COLORS 3
#define BIT_DEPTH 8

using AVT::VmbAPI::FramePtr;
using AVT::VmbAPI::CameraPtrVector;

AsynchronousGrab::AsynchronousGrab( QWidget *parent, Qt::WindowFlags flags )
    : QMainWindow( parent, flags )
    , m_bIsStreaming( false )
{
    ui.setupUi( this );
    ui.m_LabelStream->setAlignment(Qt::AlignCenter );
    // Connect GUI events with event handlers
    QObject::connect( ui.m_ButtonStartStop, SIGNAL( clicked() ), this, SLOT( OnBnClickedButtonStartstop() ) );

    // Start Vimba
    VmbErrorType err = m_ApiController.StartUp();
    setWindowTitle( QString( "AsynchronousGrab (Qt version) Vimba Version V" )+ QString::fromStdString( m_ApiController.GetVersion() ) );
    Log( "Starting Vimba", err );

    if( VmbErrorSuccess == err )
    {
        // Connect new camera found event with event handler
        QObject::connect( m_ApiController.GetCameraObserver(), SIGNAL( CameraListChangedSignal(int) ), this, SLOT( OnCameraListChanged(int) ) );

        // Initially get all connected cameras
        UpdateCameraListBox();
        std::stringstream strMsg;
        strMsg << "Cameras found..." << m_cameras.size();
        Log(strMsg.str() );
    }
}

AsynchronousGrab::~AsynchronousGrab()
{
    // if we are streaming stop streaming
    if( true == m_bIsStreaming )
        OnBnClickedButtonStartstop();

    // Before we close the application we stop Vimba
    m_ApiController.ShutDown();

}

void AsynchronousGrab::OnBnClickedButtonStartstop()
{
    VmbErrorType err;
    int nRow = ui.m_ListBoxCameras->currentRow();

    if( -1 < nRow )
    {
        if( false == m_bIsStreaming )
        {
            // Start acquisition
            err = m_ApiController.StartContinuousImageAcquisition( m_cameras[nRow] );
            // Set up Qt image
            if ( VmbErrorSuccess == err )
            {
                m_Image = QImage( m_ApiController.GetWidth(),
                                  m_ApiController.GetHeight(),
                                  QImage::Format_RGB888 );

                QObject::connect( m_ApiController.GetFrameObserver(), SIGNAL( FrameReceivedSignal(int) ), this, SLOT( OnFrameReady(int) ) );
            }
            Log( "Starting Acquisition", err );
            m_bIsStreaming = VmbErrorSuccess == err;
        }
        else
        {
            m_bIsStreaming = false;
            // Stop acquisition
            err = m_ApiController.StopContinuousImageAcquisition();
            // Clear all frames that we have not picked up so far
            m_ApiController.ClearFrameQueue();
            m_Image = QImage();
            Log( "Stopping Acquisition", err );
        }

        if( false == m_bIsStreaming )
        {
            ui.m_ButtonStartStop->setText( QString( "Start Image Acquisition" ) );
        }
        else
        {
            ui.m_ButtonStartStop->setText( QString( "Stop Image Acquisition" ) );
        }
    }
}

//
// This event handler (Qt slot) is triggered through a Qt signal posted by the frame observer
//
// Parameters:
//  [in]    status          The frame receive status (complete, incomplete, ...)
//
void AsynchronousGrab::OnFrameReady( int status )
{
    if( true == m_bIsStreaming )
    {
        // Pick up frame
        FramePtr pFrame = m_ApiController.GetFrame();
        if( SP_ISNULL( pFrame ) )
        {
            Log("frame pointer is NULL, late frame ready message");
            return;
        }
        // See if it is not corrupt
        if( VmbFrameStatusComplete == status )
        {
            VmbUchar_t *pBuffer;
            VmbErrorType err = SP_ACCESS( pFrame )->GetImage( pBuffer );
            if( VmbErrorSuccess == err )
            {
                VmbUint32_t nSize;
                err = SP_ACCESS( pFrame )->GetImageSize( nSize );
                if( VmbErrorSuccess == err )
                {
                    VmbPixelFormatType ePixelFormat = m_ApiController.GetPixelFormat();
                    if( ! m_Image.isNull() )
                    {
                        // Copy it
                        // We need that because Qt might repaint the view after we have released the frame already
                        if( ui.m_ColorProcessingCheckBox->checkState()==  Qt::Checked )
                        {
                            static const VmbFloat_t Matrix[] = {    8.0f, 0.1f, 0.1f, // this matrix just makes a quick color to mono conversion
                                                                    0.1f, 0.8f, 0.1f,
                                                                    0.0f, 0.0f, 1.0f };
                            if( VmbErrorSuccess != CopyToImage( pBuffer,ePixelFormat, m_Image, Matrix ) )
                            {
                                ui.m_ColorProcessingCheckBox->setChecked( false );
                            }
                        }
                        else
                        {
                            CopyToImage( pBuffer,ePixelFormat, m_Image );
                        }

                        // Display it
                        const QSize s = ui.m_LabelStream->size() ;
                        ui.m_LabelStream->setPixmap( QPixmap::fromImage( m_Image ).scaled(s,Qt::KeepAspectRatio ) );
                    }
                }
            }
        }
        else
        {
            // If we receive an incomplete image we do nothing but logging
            Log( "Failure in receiving image", VmbErrorOther );
        }

        // And queue it to continue streaming
        m_ApiController.QueueFrame( pFrame );
    }
}

//
// This event handler (Qt slot) is triggered through a Qt signal posted by the camera observer
//
// Parameters:
//  [in]    reason          The reason why the callback of the observer was triggered (plug-in, plug-out, ...)
//
void AsynchronousGrab::OnCameraListChanged( int reason )
{
    bool bUpdateList = false;

    // We only react on new cameras being found and known cameras being unplugged
    if( AVT::VmbAPI::UpdateTriggerPluggedIn == reason )
    {
        Log( "Camera list changed. A new camera was discovered by Vimba." );
        bUpdateList = true;
    }
    else if( AVT::VmbAPI::UpdateTriggerPluggedOut == reason )
    {
        Log( "Camera list changed. A camera was disconnected from Vimba." );
        if( true == m_bIsStreaming )
        {
            OnBnClickedButtonStartstop();
        }
        bUpdateList = true;
    }

    if( true == bUpdateList )
    {
        UpdateCameraListBox();
    }

    ui.m_ButtonStartStop->setEnabled( 0 < m_cameras.size() || m_bIsStreaming );
}

//
// Copies the content of a byte buffer to a Qt image with respect to the image's alignment
//
// Parameters:
//  [in]    pInbuffer       The byte buffer as received from the cam
//  [in]    ePixelFormat    The pixel format of the frame
//  [out]   OutImage        The filled Qt image
//
VmbErrorType AsynchronousGrab::CopyToImage( VmbUchar_t *pInBuffer, VmbPixelFormat_t ePixelFormat, QImage &pOutImage, const float *Matrix /*= NULL */ )
{
    const int           nHeight = m_ApiController.GetHeight();
    const int           nWidth  = m_ApiController.GetWidth();

    VmbImage            SourceImage,DestImage;
    VmbError_t          Result;
    SourceImage.Size    = sizeof( SourceImage );
    DestImage.Size      = sizeof( DestImage );

    Result = VmbSetImageInfoFromPixelFormat( ePixelFormat, nWidth, nHeight, & SourceImage );
    if( VmbErrorSuccess != Result )
    {
        Log( "Could not set source image info", static_cast<VmbErrorType>( Result ) );
        return static_cast<VmbErrorType>( Result );
    }
    QString             OutputFormat;
    const int           bytes_per_line = pOutImage.bytesPerLine();
    switch( pOutImage.format() )
    {
    default:
        Log( "unknown output format",VmbErrorBadParameter );
        return VmbErrorBadParameter;
    case QImage::Format_RGB888:
        if( nWidth*3 != bytes_per_line )
        {
            Log( "image transform does not support stride",VmbErrorWrongType );
            return VmbErrorWrongType;
        }
        OutputFormat = "RGB24";
        break;
    }
    Result = VmbSetImageInfoFromString( OutputFormat.toStdString().c_str(), OutputFormat.length(),nWidth,nHeight, &DestImage );
    if( VmbErrorSuccess != Result )
    {
        Log( "could not set output image info",static_cast<VmbErrorType>( Result ) );
        return static_cast<VmbErrorType>( Result );
    }
    SourceImage.Data    = pInBuffer;
    DestImage.Data      = pOutImage.bits();
    // do color processing?
    if( NULL != Matrix )
    {
        VmbTransformInfo TransformParameter;
        Result = VmbSetColorCorrectionMatrix3x3( Matrix, &TransformParameter );
        if( VmbErrorSuccess == Result )
        {
            Result = VmbImageTransform( &SourceImage, &DestImage, &TransformParameter,1 );
        }
        else
        {
            Log( "could not set matrix t o transform info ", static_cast<VmbErrorType>( Result ) );
            return static_cast<VmbErrorType>( Result );
        }
    }
    else
    {
        Result = VmbImageTransform( &SourceImage, &DestImage,NULL,0 );
    }
    if( VmbErrorSuccess != Result )
    {
        Log( "could not transform image", static_cast<VmbErrorType>( Result ) );
        return static_cast<VmbErrorType>( Result );
    }
    return static_cast<VmbErrorType>( Result );
}

//
// Queries and lists all known camera
//
void AsynchronousGrab::UpdateCameraListBox()
{
    // Get all cameras currently connected to Vimba
    CameraPtrVector cameras = m_ApiController.GetCameraList();

    // Simply forget about all cameras known so far
    ui.m_ListBoxCameras->clear();
    m_cameras.clear();

    // And query the camera details again
    for( CameraPtrVector::const_iterator iter = cameras.begin();
            cameras.end() != iter;
            ++iter )
    {
        std::string strCameraName;
        std::string strCameraID;
        if( VmbErrorSuccess != (*iter)->GetName( strCameraName ) )
        {
            strCameraName = "[NoName]";
        }
        // If for any reason we cannot get the ID of a camera we skip it
        if( VmbErrorSuccess == (*iter)->GetID( strCameraID ) )
        {
            ui.m_ListBoxCameras->addItem( QString::fromStdString( strCameraName + " " +strCameraID ) );
            m_cameras.push_back( strCameraID );
        }
    }

    ui.m_ButtonStartStop->setEnabled( 0 < m_cameras.size() || m_bIsStreaming );
}

//
// Prints out a given logging string, error code and the descriptive representation of that error code
//
// Parameters:
//  [in]    strMsg          A given message to be printed out
//  [in]    eErr            The API status code
//
void AsynchronousGrab::Log( std::string strMsg, VmbErrorType eErr )
{
    strMsg += "..." + m_ApiController.ErrorCodeToMessage( eErr );
    ui.m_ListLog->insertItem( 0, QString::fromStdString( strMsg ) );
}

//
// Prints out a given logging string
//
// Parameters:
//  [in]    strMsg          A given message to be printed out
//
void AsynchronousGrab::Log( std::string strMsg)
{
    ui.m_ListLog->insertItem( 0, QString::fromStdString( strMsg ) );
}

void AsynchronousGrab::OnBtnSavedImageClicked()
{
	image_process.ResetImage(m_Image);
	QList<QImage> image_split;
	for(int i=1;i<5;i++)
	{
		QImage image_get = image_process.GetSplitImage(i);
		image_split.append(image_get);
	}
	QList<QObject*> QObjectList ;
	QObjectList = ui.m_GBox_ROIs->children();
	QList<QObject*>::iterator i;
	QList<QImage>::iterator j;
	for (i = QObjectList.begin(),j=image_split.begin(); i != QObjectList.end(); ++i,++j) 
	{
		QLabel* LabelROISave = qobject_cast<QLabel*>(*i);
		const QSize s = LabelROISave->size();
		LabelROISave->setPixmap(QPixmap::fromImage( *j ).scaled(s,Qt::KeepAspectRatio));
	}
	/*ImageLabels = */
	//const QSize s = ui.m_LabelStream->size() ;
	//ui.m_LabelSavedImage->setPixmap(QPixmap::fromImage( image_split1 ).scaled(s,Qt::KeepAspectRatio ));
}

//对活塞类别进行分类
void AsynchronousGrab::OnBtnPistonCategoryClicked()
{
	Piston_categroy current_roi_cate = image_process.DetectPistonCategory(myTrainedBow,myTrainedSVM,1);
	QList<QObject*> QObjectList ;
	//QObjectList = ui.m_GBox_Cate->children();
	//QList<QObject*>::iterator i;
	//QList<Piston_categroy>::iterator j;
	QLabel* LabelROICate = ui.m_list_label;
	QString classification;
	switch(current_roi_cate)
	{
	case Lie:
		classification = "Lie";
		break;
	case Down:
		classification = "Down";
		break;
	case Up:
		classification = "Up";
		break;
	default:
		break;
	}
	LabelROICate->setText(classification);
}

//初始化
void AsynchronousGrab::InitialTrainedSVM()
{
	std::string outputSVM = 	"SVM_Surf.xml";
	myTrainedSVM.SetOutput(outputSVM);
	myTrainedSVM.AutoTrain();
	myTrainedSVM.GetSvmParams();
}

void AsynchronousGrab::InitialTrainedBow()
{
	//路径名
	std::string vocPath = "vocabulary.xml.gz";
	std::string feature_name = "SURF";
	myTrainedBow.SetImagesPath("imageVector_train.txt");
	//设定特征分类器初始化
	myTrainedBow.SetVocabularyPath(vocPath);
	myTrainedBow.FeatureName = feature_name;
	myTrainedBow.SetFeatureDector();
	myTrainedBow.SetDescriptorExtractor();
	myTrainedBow.SetDescriptorMatchor("BruteForce");
	myTrainedBow.SetBowExtracotr();
	myTrainedBow.GetImageVector();
	if (!myTrainedBow.ReadVocabulary())
	{

		cout << "Error: Vocbulary is not inputed" << endl;
		exit(-1);
	}
	else
	{
		if (myTrainedBow.vocabulary.empty())
		{
			cout << "Error: file " << vocPath << " can not be opened to write" << endl;
			exit(-1);
		}
	}
	myTrainedBow.SetVocabularyBow();
}

void AsynchronousGrab::OnBtn_1PistonLocationClicked()
{
	Piston_categroy current_roi_cate = image_process.DetectPistonCategory(myTrainedBow,myTrainedSVM,4);
	//获取分类信息并将其显示
	QLabel* LabelROICate = ui.m_list_label;
	QString classification;
	switch(current_roi_cate)
	{
	case Lie:
		classification = "Lie";
		break;
	case Down:
		classification = "Down";
		break;
	case Up:
		classification = "Up";
		break;
	default:
		break;
	}
	LabelROICate->setText(classification);
	//获取位置和旋转信息
	QPoint current_location;
	float current_rotation=0.0f;
	image_process.GetLocation(current_roi_cate,4,current_location,current_rotation);
	QLineEdit* line_x_roi1 = ui.m_LineXROI1;
	QLineEdit* line_y_roi1 = ui.m_LineYROI1;
	line_x_roi1->setText(QString::number(current_location.x()));
	line_y_roi1->setText(QString::number(current_location.y()));
	QLineEdit* line_rot_roi1 = ui.m_LineRotROI1;
	line_rot_roi1->setText(QString::number(current_rotation));
	QImage q_image_roi = image_process.GetCurrentResultRoiImage();
	QLabel* qlabel_roi1 = ui.m_LabelSavedImage;
	//QLabel* qlabel_roi1 = ui.m_LabelSavedImage_4;
	const QSize s = qlabel_roi1->size();
	qlabel_roi1->setPixmap(QPixmap::fromImage( q_image_roi).scaled(s,Qt::KeepAspectRatio));
	image_Info = cv::Vec3f(current_location.x(),current_location.y(),current_rotation);
}

void AsynchronousGrab::OnBtnListenClicked()
{
	myServer.ListenRobot();
	connect(&myServer,SIGNAL(OnConnected()),this,SLOT(GetMessageFromClient()));
	connect(&myServer,SIGNAL(OnConnected()),this,SLOT(GetConnectStatus()));
}

void AsynchronousGrab::GetConnectStatus()
{
	if(myServer.ConnectStaturs())
	{
		ui.m_Status_Connect->setText(" Connected");
	}
	else
	{
		ui.m_Status_Connect->setText(" Disconnected");
	}
}

void AsynchronousGrab::OnBtnSendClicked()
{
	return;
}

void AsynchronousGrab::GetMessageFromClient()
{
	ui.lineEdit->setText(myServer.GetClinetMessage());
}

void AsynchronousGrab::TransformRobot()
{
	/*image_Info = cv::Vec3f(0.0,0.0,30.0);*/
	//
	cv::Vec3f robot_Info =image_process.GetRobotDestPoint(image_Info);
	ui.m_Robot_X->setText(QString::number(robot_Info[0]));
	ui.m_Robot_Y->setText(QString::number(robot_Info[1]));
	ui.m_Robot_R->setText(QString::number(robot_Info[2]));
	QString send_msg_robot = QString::number(robot_Info[0])+","+QString::number(robot_Info[1])+","+QString::number(robot_Info[2]);
	myServer.SendMessage2Robot(send_msg_robot);
}

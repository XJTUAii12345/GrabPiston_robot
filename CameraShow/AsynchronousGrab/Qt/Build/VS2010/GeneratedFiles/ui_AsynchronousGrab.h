/********************************************************************************
** Form generated from reading UI file 'AsynchronousGrab.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ASYNCHRONOUSGRAB_H
#define UI_ASYNCHRONOUSGRAB_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AsynchronousGrabClass
{
public:
    QWidget *centralWidget;
    QListWidget *m_ListBoxCameras;
    QListWidget *m_ListLog;
    QPushButton *m_ButtonStartStop;
    QLabel *m_LabelStream;
    QCheckBox *m_ColorProcessingCheckBox;
    QPushButton *m_BtnImageSaved;
    QGroupBox *m_GBox_ROIs;
    QLabel *m_LabelSavedImage_4;
    QLabel *m_LabelSavedImage_3;
    QLabel *m_LabelSavedImage_2;
    QLabel *m_LabelSavedImage;
    QPushButton *m_BtnPistonCate;
    QGroupBox *m_GBox_Cate;
    QLabel *m_list_label;
    QLabel *m_list_label_2;
    QLabel *m_list_label_3;
    QLabel *m_list_label_4;
    QGroupBox *m_GBox_Detect;
    QPushButton *m_Btn_Detect_ROI1;
    QPushButton *m_Btn_Detect_ROI2;
    QPushButton *m_Btn_Detect_ROI3;
    QPushButton *m_Btn_Detect_ROI4;
    QLineEdit *m_LineXROI1;
    QLineEdit *m_LineYROI1;
    QLineEdit *m_LineRotROI1;
    QGroupBox *m_GBox_Net;
    QPushButton *m_Btn_Listen;
    QLineEdit *lineEdit;
    QLabel *m_Label_Get;
    QLabel *m_Label_X;
    QLabel *m_Label_Y;
    QLabel *m_Label_R;
    QPushButton *m_Btn_Send;
    QLineEdit *m_Robot_X;
    QLineEdit *m_Robot_Y;
    QLineEdit *m_Robot_R;
    QPushButton *m_BtnTran;
    QLabel *m_Status_Connect;

    void setupUi(QMainWindow *AsynchronousGrabClass)
    {
        if (AsynchronousGrabClass->objectName().isEmpty())
            AsynchronousGrabClass->setObjectName(QString::fromUtf8("AsynchronousGrabClass"));
        AsynchronousGrabClass->resize(1040, 780);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AsynchronousGrabClass->sizePolicy().hasHeightForWidth());
        AsynchronousGrabClass->setSizePolicy(sizePolicy);
        AsynchronousGrabClass->setMinimumSize(QSize(1040, 780));
        AsynchronousGrabClass->setMaximumSize(QSize(1040, 780));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/AsynchronousGrabQt/AsynchronousGrab.png"), QSize(), QIcon::Normal, QIcon::Off);
        AsynchronousGrabClass->setWindowIcon(icon);
        centralWidget = new QWidget(AsynchronousGrabClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        m_ListBoxCameras = new QListWidget(centralWidget);
        m_ListBoxCameras->setObjectName(QString::fromUtf8("m_ListBoxCameras"));
        m_ListBoxCameras->setGeometry(QRect(0, 10, 231, 461));
        m_ListLog = new QListWidget(centralWidget);
        m_ListLog->setObjectName(QString::fromUtf8("m_ListLog"));
        m_ListLog->setGeometry(QRect(0, 680, 1041, 91));
        m_ButtonStartStop = new QPushButton(centralWidget);
        m_ButtonStartStop->setObjectName(QString::fromUtf8("m_ButtonStartStop"));
        m_ButtonStartStop->setGeometry(QRect(0, 490, 231, 31));
        m_LabelStream = new QLabel(centralWidget);
        m_LabelStream->setObjectName(QString::fromUtf8("m_LabelStream"));
        m_LabelStream->setGeometry(QRect(230, 10, 520, 421));
        sizePolicy.setHeightForWidth(m_LabelStream->sizePolicy().hasHeightForWidth());
        m_LabelStream->setSizePolicy(sizePolicy);
        m_LabelStream->setFrameShape(QFrame::Box);
        m_ColorProcessingCheckBox = new QCheckBox(centralWidget);
        m_ColorProcessingCheckBox->setObjectName(QString::fromUtf8("m_ColorProcessingCheckBox"));
        m_ColorProcessingCheckBox->setGeometry(QRect(110, 470, 121, 20));
        m_BtnImageSaved = new QPushButton(centralWidget);
        m_BtnImageSaved->setObjectName(QString::fromUtf8("m_BtnImageSaved"));
        m_BtnImageSaved->setGeometry(QRect(910, 470, 111, 31));
        m_GBox_ROIs = new QGroupBox(centralWidget);
        m_GBox_ROIs->setObjectName(QString::fromUtf8("m_GBox_ROIs"));
        m_GBox_ROIs->setGeometry(QRect(750, 30, 281, 331));
        m_LabelSavedImage_4 = new QLabel(m_GBox_ROIs);
        m_LabelSavedImage_4->setObjectName(QString::fromUtf8("m_LabelSavedImage_4"));
        m_LabelSavedImage_4->setGeometry(QRect(10, 30, 128, 128));
        sizePolicy.setHeightForWidth(m_LabelSavedImage_4->sizePolicy().hasHeightForWidth());
        m_LabelSavedImage_4->setSizePolicy(sizePolicy);
        m_LabelSavedImage_4->setFrameShape(QFrame::Box);
        m_LabelSavedImage_4->setScaledContents(true);
        m_LabelSavedImage_3 = new QLabel(m_GBox_ROIs);
        m_LabelSavedImage_3->setObjectName(QString::fromUtf8("m_LabelSavedImage_3"));
        m_LabelSavedImage_3->setGeometry(QRect(10, 170, 128, 128));
        sizePolicy.setHeightForWidth(m_LabelSavedImage_3->sizePolicy().hasHeightForWidth());
        m_LabelSavedImage_3->setSizePolicy(sizePolicy);
        m_LabelSavedImage_3->setFrameShape(QFrame::Box);
        m_LabelSavedImage_3->setScaledContents(true);
        m_LabelSavedImage_2 = new QLabel(m_GBox_ROIs);
        m_LabelSavedImage_2->setObjectName(QString::fromUtf8("m_LabelSavedImage_2"));
        m_LabelSavedImage_2->setGeometry(QRect(140, 30, 128, 128));
        sizePolicy.setHeightForWidth(m_LabelSavedImage_2->sizePolicy().hasHeightForWidth());
        m_LabelSavedImage_2->setSizePolicy(sizePolicy);
        m_LabelSavedImage_2->setFrameShape(QFrame::Box);
        m_LabelSavedImage_2->setScaledContents(true);
        m_LabelSavedImage = new QLabel(m_GBox_ROIs);
        m_LabelSavedImage->setObjectName(QString::fromUtf8("m_LabelSavedImage"));
        m_LabelSavedImage->setGeometry(QRect(140, 170, 128, 128));
        sizePolicy.setHeightForWidth(m_LabelSavedImage->sizePolicy().hasHeightForWidth());
        m_LabelSavedImage->setSizePolicy(sizePolicy);
        m_LabelSavedImage->setFrameShape(QFrame::Box);
        m_LabelSavedImage->setScaledContents(true);
        m_BtnPistonCate = new QPushButton(centralWidget);
        m_BtnPistonCate->setObjectName(QString::fromUtf8("m_BtnPistonCate"));
        m_BtnPistonCate->setGeometry(QRect(930, 380, 81, 31));
        m_GBox_Cate = new QGroupBox(centralWidget);
        m_GBox_Cate->setObjectName(QString::fromUtf8("m_GBox_Cate"));
        m_GBox_Cate->setGeometry(QRect(250, 450, 481, 101));
        m_list_label = new QLabel(m_GBox_Cate);
        m_list_label->setObjectName(QString::fromUtf8("m_list_label"));
        m_list_label->setGeometry(QRect(10, 20, 71, 31));
        m_list_label_2 = new QLabel(m_GBox_Cate);
        m_list_label_2->setObjectName(QString::fromUtf8("m_list_label_2"));
        m_list_label_2->setGeometry(QRect(230, 20, 71, 31));
        m_list_label_3 = new QLabel(m_GBox_Cate);
        m_list_label_3->setObjectName(QString::fromUtf8("m_list_label_3"));
        m_list_label_3->setGeometry(QRect(10, 60, 71, 31));
        m_list_label_4 = new QLabel(m_GBox_Cate);
        m_list_label_4->setObjectName(QString::fromUtf8("m_list_label_4"));
        m_list_label_4->setGeometry(QRect(230, 60, 71, 31));
        m_GBox_Detect = new QGroupBox(centralWidget);
        m_GBox_Detect->setObjectName(QString::fromUtf8("m_GBox_Detect"));
        m_GBox_Detect->setGeometry(QRect(760, 400, 120, 161));
        m_Btn_Detect_ROI1 = new QPushButton(m_GBox_Detect);
        m_Btn_Detect_ROI1->setObjectName(QString::fromUtf8("m_Btn_Detect_ROI1"));
        m_Btn_Detect_ROI1->setGeometry(QRect(20, 20, 75, 23));
        m_Btn_Detect_ROI2 = new QPushButton(m_GBox_Detect);
        m_Btn_Detect_ROI2->setObjectName(QString::fromUtf8("m_Btn_Detect_ROI2"));
        m_Btn_Detect_ROI2->setGeometry(QRect(20, 50, 75, 23));
        m_Btn_Detect_ROI3 = new QPushButton(m_GBox_Detect);
        m_Btn_Detect_ROI3->setObjectName(QString::fromUtf8("m_Btn_Detect_ROI3"));
        m_Btn_Detect_ROI3->setGeometry(QRect(20, 80, 75, 23));
        m_Btn_Detect_ROI4 = new QPushButton(m_GBox_Detect);
        m_Btn_Detect_ROI4->setObjectName(QString::fromUtf8("m_Btn_Detect_ROI4"));
        m_Btn_Detect_ROI4->setGeometry(QRect(20, 110, 75, 23));
        m_LineXROI1 = new QLineEdit(centralWidget);
        m_LineXROI1->setObjectName(QString::fromUtf8("m_LineXROI1"));
        m_LineXROI1->setGeometry(QRect(250, 550, 61, 20));
        m_LineYROI1 = new QLineEdit(centralWidget);
        m_LineYROI1->setObjectName(QString::fromUtf8("m_LineYROI1"));
        m_LineYROI1->setGeometry(QRect(320, 550, 61, 20));
        m_LineRotROI1 = new QLineEdit(centralWidget);
        m_LineRotROI1->setObjectName(QString::fromUtf8("m_LineRotROI1"));
        m_LineRotROI1->setGeometry(QRect(250, 570, 113, 20));
        m_GBox_Net = new QGroupBox(centralWidget);
        m_GBox_Net->setObjectName(QString::fromUtf8("m_GBox_Net"));
        m_GBox_Net->setGeometry(QRect(420, 560, 371, 111));
        m_Btn_Listen = new QPushButton(m_GBox_Net);
        m_Btn_Listen->setObjectName(QString::fromUtf8("m_Btn_Listen"));
        m_Btn_Listen->setGeometry(QRect(290, 20, 75, 23));
        lineEdit = new QLineEdit(m_GBox_Net);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(110, 20, 113, 20));
        m_Label_Get = new QLabel(m_GBox_Net);
        m_Label_Get->setObjectName(QString::fromUtf8("m_Label_Get"));
        m_Label_Get->setGeometry(QRect(10, 20, 71, 16));
        m_Label_X = new QLabel(m_GBox_Net);
        m_Label_X->setObjectName(QString::fromUtf8("m_Label_X"));
        m_Label_X->setGeometry(QRect(10, 60, 54, 16));
        m_Label_Y = new QLabel(m_GBox_Net);
        m_Label_Y->setObjectName(QString::fromUtf8("m_Label_Y"));
        m_Label_Y->setGeometry(QRect(10, 80, 31, 16));
        m_Label_R = new QLabel(m_GBox_Net);
        m_Label_R->setObjectName(QString::fromUtf8("m_Label_R"));
        m_Label_R->setGeometry(QRect(160, 60, 54, 12));
        m_Btn_Send = new QPushButton(m_GBox_Net);
        m_Btn_Send->setObjectName(QString::fromUtf8("m_Btn_Send"));
        m_Btn_Send->setGeometry(QRect(290, 80, 75, 23));
        m_Robot_X = new QLineEdit(m_GBox_Net);
        m_Robot_X->setObjectName(QString::fromUtf8("m_Robot_X"));
        m_Robot_X->setGeometry(QRect(50, 60, 71, 20));
        m_Robot_Y = new QLineEdit(m_GBox_Net);
        m_Robot_Y->setObjectName(QString::fromUtf8("m_Robot_Y"));
        m_Robot_Y->setGeometry(QRect(50, 80, 71, 20));
        m_Robot_R = new QLineEdit(m_GBox_Net);
        m_Robot_R->setObjectName(QString::fromUtf8("m_Robot_R"));
        m_Robot_R->setGeometry(QRect(160, 80, 81, 20));
        m_BtnTran = new QPushButton(m_GBox_Net);
        m_BtnTran->setObjectName(QString::fromUtf8("m_BtnTran"));
        m_BtnTran->setGeometry(QRect(290, 50, 75, 23));
        m_Status_Connect = new QLabel(centralWidget);
        m_Status_Connect->setObjectName(QString::fromUtf8("m_Status_Connect"));
        m_Status_Connect->setGeometry(QRect(20, 590, 211, 31));
        AsynchronousGrabClass->setCentralWidget(centralWidget);

        retranslateUi(AsynchronousGrabClass);
        QObject::connect(m_BtnImageSaved, SIGNAL(clicked()), AsynchronousGrabClass, SLOT(OnBtnSavedImageClicked()));
        QObject::connect(m_BtnPistonCate, SIGNAL(clicked()), AsynchronousGrabClass, SLOT(OnBtnPistonCategoryClicked()));
        QObject::connect(m_Btn_Detect_ROI1, SIGNAL(clicked()), AsynchronousGrabClass, SLOT(OnBtn_1PistonLocationClicked()));
        QObject::connect(m_Btn_Listen, SIGNAL(clicked()), AsynchronousGrabClass, SLOT(OnBtnListenClicked()));
        QObject::connect(m_Btn_Send, SIGNAL(clicked()), AsynchronousGrabClass, SLOT(OnBtnListenClicked()));
        QObject::connect(m_BtnTran, SIGNAL(clicked()), AsynchronousGrabClass, SLOT(TransformRobot()));

        QMetaObject::connectSlotsByName(AsynchronousGrabClass);
    } // setupUi

    void retranslateUi(QMainWindow *AsynchronousGrabClass)
    {
        AsynchronousGrabClass->setWindowTitle(QApplication::translate("AsynchronousGrabClass", "AsynchronousGrab (Qt version)", 0, QApplication::UnicodeUTF8));
        m_ButtonStartStop->setText(QApplication::translate("AsynchronousGrabClass", "Start Image Acquisition", 0, QApplication::UnicodeUTF8));
        m_LabelStream->setText(QString());
        m_ColorProcessingCheckBox->setText(QApplication::translate("AsynchronousGrabClass", "ColorProcessing", 0, QApplication::UnicodeUTF8));
        m_BtnImageSaved->setText(QApplication::translate("AsynchronousGrabClass", "Save Image", 0, QApplication::UnicodeUTF8));
        m_GBox_ROIs->setTitle(QApplication::translate("AsynchronousGrabClass", "ROI_Image", 0, QApplication::UnicodeUTF8));
        m_LabelSavedImage_4->setText(QString());
        m_LabelSavedImage_3->setText(QString());
        m_LabelSavedImage_2->setText(QString());
        m_LabelSavedImage->setText(QString());
        m_BtnPistonCate->setText(QApplication::translate("AsynchronousGrabClass", "Category", 0, QApplication::UnicodeUTF8));
        m_GBox_Cate->setTitle(QApplication::translate("AsynchronousGrabClass", "ROI \347\261\273\345\210\253", 0, QApplication::UnicodeUTF8));
        m_list_label->setText(QApplication::translate("AsynchronousGrabClass", "ROI1:\347\261\273\345\210\253", 0, QApplication::UnicodeUTF8));
        m_list_label_2->setText(QApplication::translate("AsynchronousGrabClass", "ROI2:\347\261\273\345\210\253", 0, QApplication::UnicodeUTF8));
        m_list_label_3->setText(QApplication::translate("AsynchronousGrabClass", "ROI3:\347\261\273\345\210\253", 0, QApplication::UnicodeUTF8));
        m_list_label_4->setText(QApplication::translate("AsynchronousGrabClass", "ROI4:\347\261\273\345\210\253", 0, QApplication::UnicodeUTF8));
        m_GBox_Detect->setTitle(QApplication::translate("AsynchronousGrabClass", "\346\243\200\346\265\213\346\214\211\351\222\256", 0, QApplication::UnicodeUTF8));
        m_Btn_Detect_ROI1->setText(QApplication::translate("AsynchronousGrabClass", "ROI1", 0, QApplication::UnicodeUTF8));
        m_Btn_Detect_ROI2->setText(QApplication::translate("AsynchronousGrabClass", "ROI2", 0, QApplication::UnicodeUTF8));
        m_Btn_Detect_ROI3->setText(QApplication::translate("AsynchronousGrabClass", "ROI3", 0, QApplication::UnicodeUTF8));
        m_Btn_Detect_ROI4->setText(QApplication::translate("AsynchronousGrabClass", "ROI4", 0, QApplication::UnicodeUTF8));
        m_LineXROI1->setText(QApplication::translate("AsynchronousGrabClass", "X:", 0, QApplication::UnicodeUTF8));
        m_LineYROI1->setText(QApplication::translate("AsynchronousGrabClass", "Y:", 0, QApplication::UnicodeUTF8));
        m_LineRotROI1->setText(QApplication::translate("AsynchronousGrabClass", "Rotation:", 0, QApplication::UnicodeUTF8));
        m_GBox_Net->setTitle(QApplication::translate("AsynchronousGrabClass", "\346\234\272\345\231\250\344\272\272\344\277\241\345\217\267\344\274\240\350\276\223", 0, QApplication::UnicodeUTF8));
        m_Btn_Listen->setText(QApplication::translate("AsynchronousGrabClass", "\347\233\221\345\220\254", 0, QApplication::UnicodeUTF8));
        m_Label_Get->setText(QApplication::translate("AsynchronousGrabClass", "\350\216\267\345\217\226\347\232\204\344\277\241\345\217\267", 0, QApplication::UnicodeUTF8));
        m_Label_X->setText(QApplication::translate("AsynchronousGrabClass", "X\344\275\215\347\247\273", 0, QApplication::UnicodeUTF8));
        m_Label_Y->setText(QApplication::translate("AsynchronousGrabClass", "Y\344\275\215\347\247\273", 0, QApplication::UnicodeUTF8));
        m_Label_R->setText(QApplication::translate("AsynchronousGrabClass", "\346\227\213\350\275\254\350\247\222\345\272\246", 0, QApplication::UnicodeUTF8));
        m_Btn_Send->setText(QApplication::translate("AsynchronousGrabClass", "\345\217\221\351\200\201", 0, QApplication::UnicodeUTF8));
        m_BtnTran->setText(QApplication::translate("AsynchronousGrabClass", "\350\275\254\346\215\242\345\235\220\346\240\207", 0, QApplication::UnicodeUTF8));
        m_Status_Connect->setText(QApplication::translate("AsynchronousGrabClass", "\350\277\236\346\216\245\347\212\266\346\200\201\357\274\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AsynchronousGrabClass: public Ui_AsynchronousGrabClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASYNCHRONOUSGRAB_H

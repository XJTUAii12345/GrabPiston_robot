/********************************************************************************
** Form generated from reading UI file 'tcp_test.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCP_TEST_H
#define UI_TCP_TEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_tcp_testClass
{
public:
    QLabel *m_StatusLabel;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *m_GetLineEdit;
    QLineEdit *m_SendLineEdit;
    QPushButton *m_BtnSend;

    void setupUi(QWidget *tcp_testClass)
    {
        if (tcp_testClass->objectName().isEmpty())
            tcp_testClass->setObjectName(QString::fromUtf8("tcp_testClass"));
        tcp_testClass->resize(600, 400);
        m_StatusLabel = new QLabel(tcp_testClass);
        m_StatusLabel->setObjectName(QString::fromUtf8("m_StatusLabel"));
        m_StatusLabel->setGeometry(QRect(70, 100, 211, 81));
        label = new QLabel(tcp_testClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 210, 81, 31));
        label_2 = new QLabel(tcp_testClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 280, 81, 31));
        m_GetLineEdit = new QLineEdit(tcp_testClass);
        m_GetLineEdit->setObjectName(QString::fromUtf8("m_GetLineEdit"));
        m_GetLineEdit->setGeometry(QRect(150, 210, 113, 20));
        m_SendLineEdit = new QLineEdit(tcp_testClass);
        m_SendLineEdit->setObjectName(QString::fromUtf8("m_SendLineEdit"));
        m_SendLineEdit->setGeometry(QRect(140, 290, 113, 20));
        m_BtnSend = new QPushButton(tcp_testClass);
        m_BtnSend->setObjectName(QString::fromUtf8("m_BtnSend"));
        m_BtnSend->setGeometry(QRect(430, 300, 75, 23));

        retranslateUi(tcp_testClass);
        QObject::connect(m_BtnSend, SIGNAL(clicked()), tcp_testClass, SLOT(SendMessage()));

        QMetaObject::connectSlotsByName(tcp_testClass);
    } // setupUi

    void retranslateUi(QWidget *tcp_testClass)
    {
        tcp_testClass->setWindowTitle(QApplication::translate("tcp_testClass", "tcp_test", 0, QApplication::UnicodeUTF8));
        m_StatusLabel->setText(QApplication::translate("tcp_testClass", "\347\255\211\345\276\205\350\277\236\346\216\245\357\274\232", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("tcp_testClass", "\350\216\267\345\217\226\347\232\204\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("tcp_testClass", "\345\217\221\351\200\201\347\232\204\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        m_BtnSend->setText(QApplication::translate("tcp_testClass", "Send", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class tcp_testClass: public Ui_tcp_testClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCP_TEST_H

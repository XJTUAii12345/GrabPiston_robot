/********************************************************************************
** Form generated from reading UI file 'tcp_client.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCP_CLIENT_H
#define UI_TCP_CLIENT_H

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

class Ui_tcp_clientClass
{
public:
    QLabel *label;
    QLabel *label_2;
    QLineEdit *m_hostLineEdit;
    QLineEdit *m_portLineEdit;
    QLabel *messageLabel;
    QPushButton *m_BtnConnect;
    QPushButton *m_Send;
    QLabel *m_label;

    void setupUi(QWidget *tcp_clientClass)
    {
        if (tcp_clientClass->objectName().isEmpty())
            tcp_clientClass->setObjectName(QString::fromUtf8("tcp_clientClass"));
        tcp_clientClass->resize(600, 400);
        label = new QLabel(tcp_clientClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 70, 61, 31));
        label_2 = new QLabel(tcp_clientClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 130, 61, 31));
        m_hostLineEdit = new QLineEdit(tcp_clientClass);
        m_hostLineEdit->setObjectName(QString::fromUtf8("m_hostLineEdit"));
        m_hostLineEdit->setGeometry(QRect(120, 70, 113, 20));
        m_portLineEdit = new QLineEdit(tcp_clientClass);
        m_portLineEdit->setObjectName(QString::fromUtf8("m_portLineEdit"));
        m_portLineEdit->setGeometry(QRect(110, 140, 113, 20));
        messageLabel = new QLabel(tcp_clientClass);
        messageLabel->setObjectName(QString::fromUtf8("messageLabel"));
        messageLabel->setGeometry(QRect(60, 200, 171, 101));
        m_BtnConnect = new QPushButton(tcp_clientClass);
        m_BtnConnect->setObjectName(QString::fromUtf8("m_BtnConnect"));
        m_BtnConnect->setGeometry(QRect(440, 320, 75, 23));
        m_Send = new QPushButton(tcp_clientClass);
        m_Send->setObjectName(QString::fromUtf8("m_Send"));
        m_Send->setGeometry(QRect(440, 270, 75, 23));
        m_label = new QLabel(tcp_clientClass);
        m_label->setObjectName(QString::fromUtf8("m_label"));
        m_label->setGeometry(QRect(70, 300, 61, 31));

        retranslateUi(tcp_clientClass);
        QObject::connect(m_BtnConnect, SIGNAL(clicked()), tcp_clientClass, SLOT(on_ConnectButton_clicked()));
        QObject::connect(m_Send, SIGNAL(clicked()), tcp_clientClass, SLOT(sendMessage()));

        QMetaObject::connectSlotsByName(tcp_clientClass);
    } // setupUi

    void retranslateUi(QWidget *tcp_clientClass)
    {
        tcp_clientClass->setWindowTitle(QApplication::translate("tcp_clientClass", "tcp_client", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("tcp_clientClass", "\344\270\273\346\234\272\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("tcp_clientClass", "\347\253\257\345\217\243\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
        messageLabel->setText(QApplication::translate("tcp_clientClass", "\346\216\245\345\217\227\347\232\204\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        m_BtnConnect->setText(QApplication::translate("tcp_clientClass", "\350\277\236\346\216\245", 0, QApplication::UnicodeUTF8));
        m_Send->setText(QApplication::translate("tcp_clientClass", "\345\217\221\351\200\201", 0, QApplication::UnicodeUTF8));
        m_label->setText(QApplication::translate("tcp_clientClass", "\347\253\257\345\217\243\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class tcp_clientClass: public Ui_tcp_clientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCP_CLIENT_H

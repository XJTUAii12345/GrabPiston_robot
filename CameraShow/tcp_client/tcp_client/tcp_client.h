#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QtGui/QWidget>
#include "ui_tcp_client.h"
#include <QtNetwork>

class tcp_client : public QWidget
{
	Q_OBJECT

public:
	tcp_client(QWidget *parent = 0, Qt::WFlags flags = 0);
	~tcp_client();

private:
	Ui::tcp_clientClass ui;
	QTcpSocket *tcpSocket;  
	QString message;  //存放从服务器接收到的字符串  
	quint16 blockSize;  //存放文件的大小信息 

	private slots:  
		void newConnect(); //连接服务器  
		void readMessage();  //接收数据  
		void displayError(QAbstractSocket::SocketError);  //显示错误 
		void on_ConnectButton_clicked() ;
		void sendMessage();
};

#endif // TCP_CLIENT_H

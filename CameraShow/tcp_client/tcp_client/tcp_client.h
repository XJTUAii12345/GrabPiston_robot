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
	QString message;  //��Ŵӷ��������յ����ַ���  
	quint16 blockSize;  //����ļ��Ĵ�С��Ϣ 

	private slots:  
		void newConnect(); //���ӷ�����  
		void readMessage();  //��������  
		void displayError(QAbstractSocket::SocketError);  //��ʾ���� 
		void on_ConnectButton_clicked() ;
		void sendMessage();
};

#endif // TCP_CLIENT_H

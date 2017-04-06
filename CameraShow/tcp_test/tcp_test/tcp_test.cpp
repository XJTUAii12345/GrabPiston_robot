#include "tcp_test.h"

tcp_test::tcp_test(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	
	//�����źź���Ӧ�ۺ��� 
	ui.setupUi(this);
	tcpServer = new QTcpServer(this);  
	if(!tcpServer->listen(QHostAddress("192.168.0.71"),6666))  //QHostAddress::LocalHost,6666
	{  //��������������6666�˿ڣ������������������Ϣ�����ر�  
		qDebug() << tcpServer->errorString();  
		close();  
	}  
	connect(tcpServer,SIGNAL(newConnection()),this,SLOT(AcceptConnection()));  
}

tcp_test::~tcp_test()
{

}

void tcp_test::SendMessage()
{
	QString send_m = ui.m_SendLineEdit->text();
	clientConnection->write(send_m.toLocal8Bit());
}

void tcp_test::AcceptConnection()
{
		clientConnection = tcpServer->nextPendingConnection();
		ui.m_StatusLabel->setText("connected");
		connect(clientConnection,SIGNAL(readyRead()),this,SLOT(ReadClient()));
}

void tcp_test::ReadClient()
{
	QString str = clientConnection->readAll();
	ui.m_GetLineEdit->setText(str);
}

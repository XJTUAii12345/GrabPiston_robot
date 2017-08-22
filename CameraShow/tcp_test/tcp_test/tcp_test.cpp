#include "tcp_test.h"

tcp_test::tcp_test(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	
	//连接信号和相应槽函数 
	ui.setupUi(this);
	tcpServer = new QTcpServer(this);  
	if(!tcpServer->listen(QHostAddress("192.168.0.71"),6666))  //QHostAddress::LocalHost,6666
	{  //监听本地主机的6666端口，如果出错就输出错误信息，并关闭  
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

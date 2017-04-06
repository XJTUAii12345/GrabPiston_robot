#include "myserver.h"

MyServer::MyServer( QWidget *parent /*= 0*/, Qt::WFlags flags /*= 0*/ )	: QWidget(parent, flags)
{
	client_connect = false;
}


MyServer::~MyServer()
{

}

void MyServer::SendMessage()
{
	
	clientConnection->write(sendmsg.toLocal8Bit());
}

void MyServer::AcceptConnection()
{
	clientConnection = tcpServer->nextPendingConnection();
	client_connect = true;
	connect(clientConnection,SIGNAL(readyRead()),this,SLOT(ReadClient()));
}

void MyServer::ReadClient()
{
	QString str = clientConnection->readAll();
	get_msg = str;
	emit OnConnected();
	if(str.compare("setup_ok")==0)
	{
		SendMessage();
	}
}


bool MyServer::ConnectStaturs()
{
	return client_connect;
}

void MyServer::SendMessage2Robot( QString msg )
{
	if(!msg.isEmpty())
	{
		sendmsg = msg;
	}
	return;
}

void MyServer::ListenRobot()
{
	tcpServer = new QTcpServer();  
	if(!tcpServer->listen(QHostAddress("192.168.0.71"),6666))  //QHostAddress::LocalHost,6666
	{  //监听本地主机的6666端口，如果出错就输出错误信息，并关闭  
		qDebug() << tcpServer->errorString();  
		//close();  
	}  
	connect(tcpServer,SIGNAL(newConnection()),this,SLOT(AcceptConnection()));  
	emit OnConnected();
}

QString MyServer::GetClinetMessage()
{
	if(!get_msg.isEmpty())
	{
		return get_msg;
	}
	else
	{
		return "No valid message";
	}
}


#include "tcp_client.h"

tcp_client::tcp_client(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	tcpSocket = new QTcpSocket(this);  
	connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));  
	connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),  
		this,SLOT(displayError(QAbstractSocket::SocketError))); 
	ui.setupUi(this);
}

tcp_client::~tcp_client()
{

}

void tcp_client::newConnect()
{
	blockSize = 0; //初始化其为0  
	tcpSocket->abort(); //取消已有的连接  
	tcpSocket->connectToHost(ui.m_hostLineEdit->text(),  
		ui.m_portLineEdit->text().toInt());
	sendMessage();
	//连接到主机，这里从界面获取主机地址和端口号  
}

void tcp_client::readMessage()
{
	QDataStream in(tcpSocket);  
	in.setVersion(QDataStream::Qt_4_8);  
	//设置数据流版本，这里要和服务器端相同  
	if(blockSize==0) //如果是刚开始接收数据  
	{  
		//判断接收的数据是否有两字节，也就是文件的大小信息  
		//如果有则保存到blockSize变量中，没有则返回，继续接收数据  
		if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;  
		in >> blockSize;  
	}  
	//if(tcpSocket->bytesAvailable() < blockSize) return;  
	//如果没有得到全部的数据，则返回，继续接收数据  
	in >> message;  
	//将接收到的数据存放到变量中  
	ui.messageLabel->setText(message);  
	//显示接收到的数据  
}

void tcp_client::displayError( QAbstractSocket::SocketError )
{
	qDebug() << tcpSocket->errorString(); //输出错误信息  
}

void tcp_client::on_ConnectButton_clicked()
{
	newConnect(); //请求连接  
}

void tcp_client::sendMessage()
{
	char *data="hello qt!";
	tcpSocket->write(data);
	ui.m_label->setText("Send it");
}

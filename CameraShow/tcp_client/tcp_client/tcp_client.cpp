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
	blockSize = 0; //��ʼ����Ϊ0  
	tcpSocket->abort(); //ȡ�����е�����  
	tcpSocket->connectToHost(ui.m_hostLineEdit->text(),  
		ui.m_portLineEdit->text().toInt());
	sendMessage();
	//���ӵ�����������ӽ����ȡ������ַ�Ͷ˿ں�  
}

void tcp_client::readMessage()
{
	QDataStream in(tcpSocket);  
	in.setVersion(QDataStream::Qt_4_8);  
	//�����������汾������Ҫ�ͷ���������ͬ  
	if(blockSize==0) //����Ǹտ�ʼ��������  
	{  
		//�жϽ��յ������Ƿ������ֽڣ�Ҳ�����ļ��Ĵ�С��Ϣ  
		//������򱣴浽blockSize�����У�û���򷵻أ�������������  
		if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;  
		in >> blockSize;  
	}  
	//if(tcpSocket->bytesAvailable() < blockSize) return;  
	//���û�еõ�ȫ�������ݣ��򷵻أ�������������  
	in >> message;  
	//�����յ������ݴ�ŵ�������  
	ui.messageLabel->setText(message);  
	//��ʾ���յ�������  
}

void tcp_client::displayError( QAbstractSocket::SocketError )
{
	qDebug() << tcpSocket->errorString(); //���������Ϣ  
}

void tcp_client::on_ConnectButton_clicked()
{
	newConnect(); //��������  
}

void tcp_client::sendMessage()
{
	char *data="hello qt!";
	tcpSocket->write(data);
	ui.m_label->setText("Send it");
}

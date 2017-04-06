#ifndef MYSERVER_H
#define MYSERVER_H

#include <QtNetwork/QtNetwork>
#include <QtGui/QWidget>
class MyServer : public QWidget
{
	Q_OBJECT

public:
	MyServer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MyServer();
	QString GetClinetMessage();
	bool ConnectStaturs();
	void SendMessage2Robot(QString msg);
	void ListenRobot();
signals:
	void OnConnected();
private:
	QTcpServer *tcpServer;
	QTcpSocket *clientConnection;
	QString sendmsg;
	bool client_connect;
	QString get_msg;
	private slots:
		void SendMessage();
		void AcceptConnection();
		void ReadClient();
};

#endif // Q_MYSERVER_H

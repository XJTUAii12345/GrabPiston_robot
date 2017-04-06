#ifndef TCP_TEST_H
#define TCP_TEST_H

#include <QtGui/QWidget>
#include <QtNetwork/QtNetwork>
#include "ui_tcp_test.h"

class tcp_test : public QWidget
{
	Q_OBJECT

public:
	tcp_test(QWidget *parent = 0, Qt::WFlags flags = 0);
	~tcp_test();

private:
	Ui::tcp_testClass ui;

	QTcpServer *tcpServer;
	QTcpSocket *clientConnection;
	private slots:
	void SendMessage();
	void AcceptConnection();
	void ReadClient();
};

#endif // TCP_TEST_H

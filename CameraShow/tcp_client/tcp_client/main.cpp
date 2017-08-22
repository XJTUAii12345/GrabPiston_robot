#include "tcp_client.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	tcp_client w;
	w.show();
	return a.exec();
}

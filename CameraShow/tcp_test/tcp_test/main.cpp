#include "tcp_test.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	tcp_test w;
	w.show();
	return a.exec();
}

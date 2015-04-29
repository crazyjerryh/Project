#include "qfadingwindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QFadingWindow w;
	w.show();
	return a.exec();
}

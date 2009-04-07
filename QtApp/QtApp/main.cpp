#include <QtGui/QApplication>
#include "qtapp.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtApp w;
	w.show();
	return a.exec();
}

#include <QtGui/QApplication>
#include "qtapp.h"

int main(int argc, char *argv[])
{
	try{
		QApplication a(argc, argv);
		QtApp w;
		w.show();
		return a.exec();
	}
	catch(CppSQLite3Exception &e)
	{
		qDebug("���̷����쳣�������˳�[%d/%s]%s\n",e.errorCode(),e.errorCodeAsString(e.errorCode()),e.errorMessage());
	}
	catch(...)
	{
		qDebug("���̷����쳣�������˳�\n");
	}
}

#include <QtGui/QApplication>
#include "qtapp.h"
#include <exception>
using namespace std;

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
		qDebug("进程发生异常，崩溃退出[%d/%s]%s\n",e.errorCode(),e.errorCodeAsString(e.errorCode()),e.errorMessage());
	}
	catch(exception &e)
	{
		qDebug("进程发生异常，崩溃退出[%s]\n",e.what());
	}
	catch(...)
	{
		qDebug("进程发生异常，崩溃退出\n");
	}
}

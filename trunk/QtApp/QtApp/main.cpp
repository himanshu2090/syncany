#include <QtGui/QApplication>
#include "qtapp.h"
#include <exception>
using namespace std;

class MyApp:public QApplication
{
public: 

	MyApp(int _argc, char **_argv): QApplication(_argc, _argv)
	{ 

	}
	//bool notify(QObject *receiver, QEvent *e)
	//{

	//	qDebug( "Notify()!" );

	//	return false;

	//} 

};
int main(int argc, char *argv[])
{
	QTextCodec *codec=QTextCodec::codecForName("GB18030");
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForCStrings(codec);
	try{
		MyApp a(argc, argv);
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

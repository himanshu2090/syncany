#include <QtGui/QApplication>
#include "qtapp.h"
#include <exception>
using namespace std;

//class MyApp:public QApplication
//{
//public: 
//
//	MyApp(int _argc, char **_argv): QApplication(_argc, _argv)
//	{ 
//
//	}
//	//bool notify(QObject *receiver, QEvent *e)
//	//{
//
//	//	qDebug( "Notify()!" );
//
//	//	return false;
//
//	//} 
//
//};
int main(int argc, char *argv[])
{
	try{
		QApplication a(argc, argv);
		QTextCodec *codec=QTextCodec::codecForName("GBK");
		if(codec==null)
		{
			codec=QTextCodec::codecForLocale();
			QFile qf(QDir::currentPath()+"/test.txt");
			qf.open(QIODevice::Truncate|QIODevice::ReadWrite);
			qf.write(QByteArray("error codec,use codec:")+codec->name()+"\n");
			QList<QByteArray> alias=codec->aliases();
			for(int i=0;i<alias.size();++i)
			{
				qf.write(alias[i]+"\n");
			}
			qf.close();
		}
		QTextCodec::setCodecForLocale(codec);
		QTextCodec::setCodecForTr(codec);
		QTextCodec::setCodecForCStrings(codec);

		QTranslator sys_translator;
		sys_translator.load("qt_zh_CN");
		qApp->installTranslator(&sys_translator);

		QTranslator translator;
		if(!translator.load("qtapp_zh"))
		{
			QFile qf(QDir::currentPath()+"/test.txt");
			qf.open(QIODevice::Truncate|QIODevice::ReadWrite);
			qf.write(QByteArray("error load qtapp_zh.qm\n"));
			qf.close();
		}
		qApp->installTranslator(&translator);

		QtApp w(null,Qt::CustomizeWindowHint  | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
		w.show();
		return a.exec();
	}
	catch(CppSQLite3Exception &e)
	{
		qDebug("Sqlite3Exception crashed! [%d/%s]%s\n",e.errorCode(),e.errorCodeAsString(e.errorCode()),e.errorMessage());
	}
	catch(exception &e)
	{
		qDebug("Unknown crashed! [%s]\n",e.what());
	}
	catch(...)
	{
		qDebug("Crashed !!!!\n");
	}
        return ERR_NONE;
}

#include "qtapp.h"

QtApp::QtApp(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{

	QTextCodec *codec=QTextCodec::codecForName("GB2312");
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForCStrings(codec);

	ui.setupUi(this);
	//ui.pushButton->hide();
	syncdb=SyncDB::instance();
	synconf=Synconf::instance();
	QString svrhost=synconf->getstr("server_host","5.1.193.51");
	QString svrport=synconf->getstr("server_port","18120");

	synconf->setstr("server_host",svrhost);
	synconf->setstr("server_port",svrport,true);
	ui.textHost->setText(svrhost);
	ui.textPort->setText(svrport);
	sm=new SessionManager(this);
	watcher=new LocalFileWatcher(this);

	connect(watcher.data(),SIGNAL(filesChanged(QStringList &)),this,SLOT(local_files_changed(QStringList &)));

}

QtApp::~QtApp()
{
	synconf->save_conf();
}


void QtApp::on_pushButton_clicked()
{
	//ui.textLogger->append("Hi,I'v been clicked!");
	synconf->setstr("server_host",ui.textHost->text());
	synconf->setstr("server_port",ui.textPort->text());
}

void QtApp::on_btnSend_clicked()
{
	QString strContent=ui.textInput->toPlainText();
	ui.textInput->setPlainText("");
	strContent.remove('\r');
	//为测试公开client
	sm->client.sendData(strContent+"\n");
}

void QtApp::on_btnClear_clicked()
{
	ui.textLogger->setPlainText("");
	ui.textLoggerIn->setPlainText("");
	ui.textLoggerOut->setPlainText("");
}

void QtApp::log(QString str)
{
	QString so="LOG:"+str.trimmed();
	ui.textLogger->append(so);
}

void QtApp::login(QString str)
{
	QString so="<<:"+str.trimmed();
	ui.textLoggerIn->append(so);
}

void QtApp::logout(QString str)
{
	QString so=">>:"+str.trimmed();
	ui.textLoggerOut->append(so);
}

void QtApp::on_btnConnect_clicked()
{
	//应该更新HOST地址和端口
	sm->ConnectHost();
}

void QtApp::on_btnDisconnect_clicked()
{
	sm->DisconnectHost();
}

void QtApp::on_pushButton_2_clicked()
{

}

void QtApp::local_files_changed(QStringList & strFiles)
{
	QString str;
	for(int i=0;i<strFiles.size();++i)
	{
		str.append(strFiles[i]);
		str.append("\n");
	}
	
	if(str!="")
		log(str);
}


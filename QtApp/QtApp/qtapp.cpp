#include "qtapp.h"

QtApp::QtApp(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
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
}

QtApp::~QtApp()
{
	synconf->save_conf();
	if(sm!=null)
		delete sm;
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
	QString so="LOG:"+str;
	ui.textLogger->append(so.toLocal8Bit().data());
}

void QtApp::login(QString str)
{
	QString so="<<:"+str;
	ui.textLoggerIn->append(so.toLocal8Bit().data());
}

void QtApp::logout(QString str)
{
	QString so=">>:"+str;
	ui.textLoggerOut->append(so.toLocal8Bit().data());
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
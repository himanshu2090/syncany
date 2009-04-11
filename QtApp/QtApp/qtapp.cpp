#include "qtapp.h"

QtApp::QtApp(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
	,sm(this)
{
	ui.setupUi(this);
	ui.pushButton->hide();
	syncdb=SyncDB::instance();
	synconf=Synconf::instance();
	QString svrhost=synconf->getstr("server_host","5.40.92.214");
	QString svrport=synconf->getstr("server_port","18120");
	synconf->setstr("server_host",svrhost);
	synconf->setstr("server_port",svrport);
	ui.textPort->setText(svrhost);
	ui.textHost->setText(svrport);
}

QtApp::~QtApp()
{

}


void QtApp::on_pushButton_clicked()
{
	ui.textLogger->append("Hi,I'v been clicked!");
}

void QtApp::on_btnSend_clicked()
{
	QString strContent=ui.textInput->toPlainText();
	ui.textInput->setPlainText("");
	strContent.remove('\r');
	sm.client.sendData(strContent+"\n");
}

void QtApp::on_btnClear_clicked()
{
	ui.textLogger->setPlainText("");
	ui.textLoggerIn->setPlainText("");
	ui.textLoggerOut->setPlainText("");
}

void QtApp::log(QString str)
{
	ui.textLogger->append("LOG:"+str);
}

void QtApp::login(QString str)
{
	ui.textLoggerIn->append("<<"+str);
}

void QtApp::logout(QString str)
{
	ui.textLoggerOut->append(">>"+str);
}

void QtApp::on_btnConnect_clicked()
{
	//Ӧ�ø���HOST��ַ�Ͷ˿�
	sm.ConnectHost();
}

void QtApp::on_btnDisconnect_clicked()
{
	sm.DisconnectHost();
}
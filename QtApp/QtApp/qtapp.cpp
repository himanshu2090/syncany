#include "qtapp.h"

QtApp::QtApp(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
	,client(this)
{
	ui.setupUi(this);
	connect(&client,SIGNAL(sigLogger(QString)),this,SLOT(log(QString)));
	connect(&client,SIGNAL(sigIn(QString)),this,SLOT(login(QString)));
	connect(&client,SIGNAL(sigOut(QString)),this,SLOT(logout(QString)));
	ui.textPort->setText("80");
	ui.textHost->setText("www.g.cn");
	ui.pushButton->hide();
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
	client.sendData(strContent+"\n");
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
	client.DisconnectHost();
	client.ConnectHost(ui.textHost->text(),ui.textPort->text().toInt());
}

void QtApp::on_btnDisconnect_clicked()
{
	client.DisconnectHost();
}
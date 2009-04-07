#include "qtapp.h"

QtApp::QtApp(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
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
	QString strInput="Send:"+strContent;
	ui.textLogger->append(strInput);
	//send content
}

void QtApp::on_btnClear_clicked()
{
	ui.textLogger->setPlainText("");
}
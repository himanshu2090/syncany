#include "optiondlg.h"

OptionDlg::OptionDlg(QWidget *parent)
	: QDialog(parent,Qt::CustomizeWindowHint  | Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
	ui.setupUi(this);
	synconf=Synconf::instance();
	synconf->save_conf();
	//此时应该连接关注synconf的keychanged事件

	QFile qf(synconf->getstr("work_dir")+"/synconf.ini");
	qf.open(QFile::ReadOnly);
	QByteArray buf=qf.readAll();
	qf.close();

	ui.plainTextEdit->setPlainText(QString::fromLocal8Bit(buf));

}

OptionDlg::~OptionDlg()
{

}


void OptionDlg::on_pushButtonOptionCancel_clicked()
{
	this->close();
}

void OptionDlg::on_pushButtonOptionOK_clicked()
{
	QFile qf(synconf->getstr("work_dir")+"/synconf.ini");
	qf.open(QFile::Truncate|QFile::ReadWrite);
	qf.write(ui.plainTextEdit->toPlainText().toLocal8Bit());
	qf.close();
	synconf->load_conf();
	this->close();
}


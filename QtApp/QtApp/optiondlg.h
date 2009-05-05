#ifndef OPTIONDLG_H
#define OPTIONDLG_H

#include <QWidget>
#include "ui_optiondlg.h"
#include "common.h"
#include "synconf.h"

class OptionDlg : public QDialog
{
	Q_OBJECT

public:
	OptionDlg(QWidget *parent = 0);
	~OptionDlg();

private:
	Ui::OptionDlgClass ui;
	Synconf *synconf;

private slots:
	void on_pushButtonOptionOK_clicked();
	void on_pushButtonOptionCancel_clicked();


};

#endif // OPTIONDLG_H

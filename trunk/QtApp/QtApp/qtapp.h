#ifndef QTAPP_H
#define QTAPP_H

#include <QtGui/QDialog>
#include "ui_qtapp.h"

class QtApp : public QDialog
{
	Q_OBJECT

public:
	QtApp(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QtApp();

private:
	Ui::QtAppClass ui;

private slots:
	void on_btnClear_clicked();
	void on_btnSend_clicked();
	void on_pushButton_clicked();
};

#endif // QTAPP_H

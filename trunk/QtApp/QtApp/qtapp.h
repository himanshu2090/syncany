#ifndef QTAPP_H
#define QTAPP_H

#include "common.h"
#include "ui_qtapp.h"
#include "sessionmanager.h"
#include "synconf.h"
#include "syncdb.h"

class QtApp : public QDialog
{
	Q_OBJECT

public:
	QtApp(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QtApp();

private:
	Ui::QtAppClass ui;
	SessionManager *sm;
	Synconf *synconf;
	SyncDB *syncdb;
private slots:
	void on_pushButton_2_clicked();
	void on_btnDisconnect_clicked();
	void on_btnConnect_clicked();
	void on_btnClear_clicked();
	void on_btnSend_clicked();
	void on_pushButton_clicked();
	void log(QString str);
	void login(QString str);
	void logout(QString str);
};

#endif // QTAPP_H

#ifndef QTAPP_H
#define QTAPP_H

#include "common.h"
#include "ui_qtapp.h"
#include "sessionmanager.h"
#include "synconf.h"
#include "syncdb.h"
#include "localfilewatcher.h"

class QtApp : public QDialog
{
	Q_OBJECT

public:
	QtApp(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QtApp();


private:
	Ui::QtAppClass ui;
	QPointer<SessionManager> sm;
	QPointer<LocalFileWatcher> watcher;
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

	void local_files_changed(QStringList &strFiles);
};

#endif // QTAPP_H

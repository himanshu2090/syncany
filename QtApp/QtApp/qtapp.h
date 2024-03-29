#ifndef QTAPP_H
#define QTAPP_H

#include "common.h"
#include "ui_qtapp.h"
#include "sessionmanager.h"
#include "synconf.h"
#include "syncdb.h"
#include "localfilewatcher.h"
#include "eventlistdlg.h"
//#include <QSystemTrayIcon>
//#include <QMenu>
//#include <QCloseEvent>
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
	//singleton class
	Synconf *synconf;
	SyncDB *syncdb;
	EventList *el;
	QPointer<EventListDlg> eventDlg;
public:
	bool bAllowClose;
private:
	QMenu *trayMenu;
	QSystemTrayIcon *trayIcon;
	QAction *quitAction;
	QAction *showEventListAction;
	QMutex m_locker_log;
private slots:
	void on_btnBrowseSyncdirectory_clicked();
	void on_pushButtonDecode_clicked();
	void on_pushButtonQuit_clicked();
	void on_pushButtonSettings_clicked();
	void on_pushButtonRegister_clicked();
	void on_pushButtonSetAccount_clicked();
	void on_pushButton_3_clicked();
	void on_pushButton_Test_Put_Get_clicked();
	void on_btnDisconnect_clicked();
	void on_btnConnect_clicked();
	void on_btnClear_clicked();
	void on_btnSend_clicked();
	void on_pushButton_clicked();
public slots:
	void log(QString str);
	void login(QString str);
	void logout(QString str);

	void recvAlertMessage(StringMap alert_props,AlertMessageList msglist);
	//������������
	void trayActived(QSystemTrayIcon::ActivationReason );
	void trayMessage(QString strTitle,QString strInfo);
	void trigger_quit();
	void trigger_showEventList();
protected:
	void timerEvent(QTimerEvent *event);
	int eventTimerID;
private:
	void createActions();
	void createTrayIcon();

protected:
	virtual void closeEvent ( QCloseEvent * event ) ;
};

#endif // QTAPP_H

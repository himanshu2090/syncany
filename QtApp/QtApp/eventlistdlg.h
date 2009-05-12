#ifndef EVENTLISTDLG_H
#define EVENTLISTDLG_H

#include <QDialog>
#include "common.h"
#include "ui_eventlistdlg.h"
#include "synconf.h"
#include "syncdb.h"
#include "EventList.h"
class EventListDlg : public QDialog
{
	Q_OBJECT

public:
	EventListDlg(QWidget *parent = 0,Qt::WindowFlags f=Qt::CustomizeWindowHint  | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
	~EventListDlg();

	void reload();
private:
	Ui::EventListDlgClass ui;
	EventList *el;
	SyncDB *syncdb;
	Synconf *synconf;

public slots:
	void itemClicked ( QTreeWidgetItem * item );
	void itemDoubleClicked ( QTreeWidgetItem * item );
	void itemEntered ( QTreeWidgetItem * item );
	void itemPressed ( QTreeWidgetItem * item );

};

#endif // EVENTLISTDLG_H

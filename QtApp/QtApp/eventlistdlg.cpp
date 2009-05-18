#include "eventlistdlg.h"
EventListDlg::EventListDlg(QWidget *parent,Qt::WindowFlags f)
	: QDialog(parent,f)
{
	ui.setupUi(this);
	synconf=Synconf::instance();
	syncdb=SyncDB::instance();
	el=EventList::instance();

	ui.listWidget->setColumnCount(4);
	QStringList strHeaders;
	strHeaders.append(QObject::tr("ID"));
	strHeaders.append(QObject::tr("Url"));
	strHeaders.append(QObject::tr("SyncDir"));
	strHeaders.append(QObject::tr("SyncOp"));
	ui.listWidget->setHeaderLabels(strHeaders);

	connect(ui.listWidget,SIGNAL(itemClicked ( QTreeWidgetItem *  ,int  )),this,SLOT(itemClicked ( QTreeWidgetItem *  ,int  )));
	connect(ui.listWidget,SIGNAL(itemDoubleClicked ( QTreeWidgetItem *  ,int  )),this,SLOT(itemDoubleClicked ( QTreeWidgetItem *  ,int  )));
	connect(ui.listWidget,SIGNAL(itemEntered ( QTreeWidgetItem *  ,int  )),this,SLOT(itemEntered ( QTreeWidgetItem *  ,int  )));
	connect(ui.listWidget,SIGNAL(itemPressed ( QTreeWidgetItem *  ,int  )),this,SLOT(itemPressed ( QTreeWidgetItem *  ,int  )));
}

EventListDlg::~EventListDlg()
{

}

void EventListDlg::reload()
{
	ui.listWidget->clear();

	int eid=-1;
	while(true)
	{
		QString strUrl;
		int syncdir;
		int syncop;
		if(el->getNextEvent(eid,strUrl,syncdir,syncop)==ERR_NODATA)
			break;
		QTreeWidgetItem *item =new QTreeWidgetItem(ui.listWidget);
		item->setText(0,QString::number(eid));
		item->setText(1,strUrl);
		item->setText(2,syncdir==SYNC_TO_SERVER?">>":"<<");
		QString strOp;
		switch(syncop)
		{
		case SYNC_OP_ADD:strOp="Add";break;
		case SYNC_OP_MODIFY:strOp="Modify";break;
		case SYNC_OP_REMOVE:strOp="Remove";break;
		case SYNC_OP_RENAME:strOp="Rename";break;
		default:strOp="Unknown";
		}
		item->setText(3,strOp);
		ui.listWidget->insertTopLevelItem(0,item);
	}
}


void EventListDlg::itemClicked ( QTreeWidgetItem * item  ,int col)
{

}
void EventListDlg::itemDoubleClicked ( QTreeWidgetItem * item  ,int col)
{

}
void EventListDlg::itemEntered ( QTreeWidgetItem * item  ,int col)
{

}
void EventListDlg::itemPressed ( QTreeWidgetItem * item  ,int col)
{

}

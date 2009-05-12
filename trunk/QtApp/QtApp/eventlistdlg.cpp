#include "eventlistdlg.h"
EventListDlg::EventListDlg(QWidget *parent,Qt::WindowFlags f)
	: QDialog(parent,f)
{
	ui.setupUi(this);
	synconf=Synconf::instance();
	syncdb=SyncDB::instance();
	el=EventList::instance();
	//connect(ui.listWidget,SIGNAL(itemClicked ( QTreeWidgetItem * item )),this,SLOT(itemClicked ( QTreeWidgetItem * item )));
	//connect(ui.listWidget,SIGNAL(itemDoubleClicked ( QTreeWidgetItem * item )),this,SLOT(itemDoubleClicked ( QTreeWidgetItem * item )));
	//connect(ui.listWidget,SIGNAL(itemEntered ( QTreeWidgetItem * item )),this,SLOT(itemEntered ( QTreeWidgetItem * item )));
	//connect(ui.listWidget,SIGNAL(itemPressed ( QTreeWidgetItem * item )),this,SLOT(itemPressed ( QTreeWidgetItem * item )));

	ui.listWidget->setColumnCount(4);
	QStringList strHeaders;
	strHeaders.append("ID");
	strHeaders.append("Url");
	strHeaders.append("SyncDir");
	strHeaders.append("SyncOp");
	ui.listWidget->setHeaderLabels(strHeaders);

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


void EventListDlg::itemClicked ( QTreeWidgetItem * item )
{

}
void EventListDlg::itemDoubleClicked ( QTreeWidgetItem * item )
{

}
void EventListDlg::itemEntered ( QTreeWidgetItem * item )
{

}
void EventListDlg::itemPressed ( QTreeWidgetItem * item )
{

}

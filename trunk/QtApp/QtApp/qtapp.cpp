#include "qtapp.h"
#include "syncbasefile.h"
#include "optiondlg.h"

QtApp::QtApp(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	bAllowClose=false;//��ֱֹ�ӹرմ���

	ui.setupUi(this);
	//ui.pushButton->hide();
	syncdb=SyncDB::instance();
	synconf=Synconf::instance();
	el=EventList::instance();
	QString svrhost=synconf->getstr(KEY_SERVER_IP,"5.152.69.23");
	QString svrport=synconf->getstr(KEY_SERVER_PORT,"18120");

	QString strUser=synconf->getstr("user_id");
	QString strPass=synconf->getstr("user_password");

	ui.lineEditUser->setText(strUser);
	ui.lineEditPassword->setText(strPass);

	synconf->setstr(KEY_SERVER_IP,svrhost);
	synconf->setstr(KEY_SERVER_PORT,svrport,true);
	ui.textHost->setText(svrhost);
	ui.textPort->setText(svrport);

	ui.txtSyncDirectory->setText(synconf->getstr(KEY_SYNCDIR,QDir::homePath()));

	sm=new SessionManager(this);
	watcher=new LocalFileWatcher(this);
	eventDlg=new EventListDlg(this,Qt::CustomizeWindowHint  | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);

	connect(watcher.data(),SIGNAL(alertMessage(StringMap,AlertMessageList)),this,SLOT(recvAlertMessage(StringMap,AlertMessageList)));
	connect(sm.data(),SIGNAL(alertMessage(StringMap,AlertMessageList)),this,SLOT(recvAlertMessage(StringMap,AlertMessageList)));

	createActions();
	createTrayIcon();


	eventTimerID=startTimer(1000);

	connect(sm.data(),SIGNAL(trayMessage(QString ,QString )),this,SLOT(trayMessage(QString ,QString)));

	trayMessage(QObject::tr("SyncAny Alert!"),QObject::tr("Double click to show MainWindow.\nSingle click to active MainWindow.\nRight click to show ContentMenu!\nClick 'Quit' to terminate program!"));
}

void QtApp::createActions()
{
	quitAction = new QAction(tr("&Quit"), this);
	quitAction->setIcon(QIcon(":/QtApp/QtApp.ico"));
	//quitAction->setIcon(QIcon("QtApp.ico"));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(trigger_quit()));
	showEventListAction = new QAction(tr("Show/Hide EventList"),this);
	connect(showEventListAction,SIGNAL(triggered()),this,SLOT(trigger_showEventList()));
}

void QtApp::createTrayIcon()
{
	trayMenu = new QMenu(this);
	trayMenu->addAction(showEventListAction);
	trayMenu->addSeparator();
	trayMenu->addAction(quitAction);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayMenu);
	QIcon icon=QIcon(":/QtApp/QtApp.ico");//QIcon(".\\QtApp.ico");
	//QIcon icon=QIcon("QtApp.ico");//QIcon(".\\QtApp.ico");
	trayIcon->setIcon(icon);
	trayIcon->setToolTip(QObject::tr("SyncAny Client Tray"));
	trayIcon->show();

	connect(trayIcon,SIGNAL(activated ( QSystemTrayIcon::ActivationReason  )),this,SLOT(trayActived(QSystemTrayIcon::ActivationReason )));


}

void QtApp::trayMessage(QString strTitle,QString strInfo)
{
	if(trayIcon!=NULL)
	{
		trayIcon->showMessage(strTitle,strInfo);
	}
}

void QtApp::trayActived(QSystemTrayIcon::ActivationReason ar)
{
	/*
	QSystemTrayIcon::Unknown	0	Unknown reason
		QSystemTrayIcon::Context	1	The context menu for the system tray entry was requested
		QSystemTrayIcon::DoubleClick	2	The system tray entry was double clicked
		QSystemTrayIcon::Trigger	3	The system tray entry was clicked
		QSystemTrayIcon::MiddleClick	4	The system tray entry was clicked with the middle mouse button
		*/
	switch(ar)
	{
        default:
            break;
	case QSystemTrayIcon::DoubleClick:
		if(isVisible())
			hide();
		else
			show();
		break;
	case QSystemTrayIcon::Context:
		trayMenu->exec();
		break;
	case QSystemTrayIcon::Trigger:
		activateWindow();
                break;
	}
}
void QtApp::trigger_showEventList()
{
	if(eventDlg->isVisible())
		eventDlg->hide();
	else
	{
		eventDlg->show();
		eventDlg->reload();
	}
}
void QtApp::trigger_quit()
{
	qApp->quit();
}

QtApp::~QtApp()
{
	synconf->save_conf();
}


void QtApp::on_pushButton_clicked()
{
	//ui.textLogger->append("Hi,I'v been clicked!");
	synconf->setstr(KEY_SERVER_IP,ui.textHost->text());
	synconf->setstr(KEY_SERVER_PORT,ui.textPort->text());
}

void QtApp::on_btnSend_clicked()
{
	QString strContent=ui.textInput->toPlainText();
	ui.textInput->setPlainText("");
	strContent.remove('\r');
	//Ϊ���Թ���client
	sm->client.sendData(strContent+"\n");
}

void QtApp::on_btnClear_clicked()
{
	ui.textLoggerIn->setPlainText("");
}

void QtApp::log(QString str)
{
	QMutexLocker autolocker(&m_locker_log);
	QString strFilename=synconf->getstr(KEY_WORK_DIR)+"/"+QDateTime::currentDateTime().toString(QString("yyyy-MM-dd"))+".txt";
	QFile qf(strFilename);
	qf.open(QIODevice::Append);
	QString so=QDateTime::currentDateTime().toString(QString("yyyy-MM-dd hh:mm:ss : ")) +"LOG:"+str.trimmed();
	qf.write((so+"\r\n").toLocal8Bit());
	qf.close();
	ui.textLoggerIn->append(so);
}

void QtApp::login(QString str)
{
	QString so="<<:"+str.trimmed();
	QString strFilename=synconf->getstr(KEY_WORK_DIR)+"/"+QDateTime::currentDateTime().toString(QString("yyyy-MM-dd"))+".txt";
	QFile qf(strFilename);
	qf.open(QIODevice::Append);
	so=QDateTime::currentDateTime().toString(QString("yyyy-MM-dd hh:mm:ss : ")) +so;
	qf.write((so+"\n").toLocal8Bit());
	qf.close();
	ui.textLoggerIn->append(so);
}

void QtApp::logout(QString str)
{
	QString so=">>:"+str.trimmed();
	QString strFilename=synconf->getstr(KEY_WORK_DIR)+"/"+QDateTime::currentDateTime().toString(QString("yyyy-MM-dd"))+".txt";
	QFile qf(strFilename);
	qf.open(QIODevice::Append);
	so=QDateTime::currentDateTime().toString(QString("yyyy-MM-dd hh:mm:ss : ")) +so;
	qf.write((so+"\n").toLocal8Bit());
	qf.close();
	ui.textLoggerIn->append(so);
}

void QtApp::on_btnConnect_clicked()
{
	//Ӧ�ø���HOST��ַ�Ͷ˿�
	sm->ConnectHost();
}

void QtApp::on_btnDisconnect_clicked()
{
	sm->DisconnectHost();
}


void QtApp::on_pushButton_Test_Put_Get_clicked()
{
	if(1)
	{
		QDir::setCurrent("c:/");
		QString strFilename="test.txt";
		QFile qf(strFilename);
		qf.open(QIODevice::ReadOnly);
		QByteArray buf=qf.readAll();
		qf.close();
		QByteArray strCmd="put "+sm->generate_cmdid().toLocal8Bit()+" url=/"+ strFilename.toLocal8Bit() +" size="+QString::number(buf.length()).toLocal8Bit()+"\n"+buf;
		sm->client.sendData(strCmd);

	}
	else
	{
		QString strFilename="/test.txt";
		QByteArray strCmd="get "+sm->generate_cmdid().toLocal8Bit()+" url=/"+ strFilename.toLocal8Bit() +"\n";
		sm->client.sendData(strCmd);
	}


}

void QtApp::on_pushButton_3_clicked()
{
	QByteArray strurl="http://www.g.cn/?abc sdf����!@#$%%^&&**(()__+";
	QByteArray s=raw_url_encode(strurl);
	qDebug(QString::fromLocal8Bit(strurl).toUtf8());
	QByteArray ss=raw_url_decode(s);
	qDebug(ss);
	qDebug("����");

	sm->ls_file("/");
}

void QtApp::closeEvent ( QCloseEvent * event ) 
{
	if(!bAllowClose)
	{
		event->ignore();
		hide();
	}
}



void QtApp::on_pushButtonSetAccount_clicked()
{
	QString strUser=ui.lineEditUser->text();
	QString strPass=ui.lineEditPassword->text();
	if(strUser=="" || strPass=="" )
		return ;
	if(synconf->getstr("user_id")=="" || synconf->getstr("user_password")=="")
	{
		//δע��
		synconf->setstr(KEY_SESSION_ID,"");
		synconf->setstr(KEY_DEVICE_ID,"");
		synconf->setstr("user_id",strUser);
		synconf->setstr("user_password",strPass,true);
	}
	else
	{
		if(QMessageBox::question(this,QObject::tr("change account?"),QObject::tr("Your device has logined with the account:")+synconf->getstr("user_id")+QObject::tr(",do you realy want change your account?(it'll erase all your old account informations!"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)== QMessageBox::Yes)
		{
			synconf->setstr(KEY_SESSION_ID,"");
			synconf->setstr(KEY_DEVICE_ID,"");
			synconf->setstr("user_id",strUser);
			synconf->setstr("user_password",strPass,true);
		}

	}
}

void QtApp::on_pushButtonRegister_clicked()
{
	QString strUser=ui.lineEditUser->text();
	QString strPass=ui.lineEditPassword->text();
	if(strUser=="" || strPass=="" )
		return ;
	if(synconf->getstr("user_id")=="" || synconf->getstr("user_password")=="")
	{
		synconf->setstr(KEY_SESSION_ID,"");
		synconf->setstr(KEY_DEVICE_ID,"");
		synconf->setstr("user_id",strUser);
		synconf->setstr("user_password",strPass);
		synconf->setstr("doregister","1",true);
	}
	else
	{
		if(QMessageBox::question(this,QObject::tr("register again?"),QObject::tr("Your device has logined with the account:")+synconf->getstr("user_id")+QObject::tr(",do you realy want change your account?(it'll erase all your old account informations!"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)== QMessageBox::Yes)
		{
			synconf->setstr(KEY_SESSION_ID,"");
			synconf->setstr(KEY_DEVICE_ID,"");
			synconf->setstr("user_id",strUser);
			synconf->setstr("user_password",strPass);
			synconf->setstr("doregister","1",true);
		}
	}
}





void QtApp::on_pushButtonSettings_clicked()
{
	OptionDlg dlg(this);
	dlg.exec();
}

void QtApp::on_pushButtonQuit_clicked()
{
	trigger_quit();
}

void QtApp::on_pushButtonDecode_clicked()
{
	QByteArray buf=ui.textUrl->text().toLocal8Bit();
	buf=raw_url_decode(buf);
	ui.textUrl->setText(QString::fromLocal8Bit(buf));
}

void QtApp::timerEvent(QTimerEvent *event)
{
	if(event->timerId()==eventTimerID)
	{
		//����¼�����
		int eid=-1;
		QString strUrl;
		int syncdir;
		int syncop;
		while(true)
		{
			if(el->getNextEvent(eid,strUrl,syncdir,syncop,false) == ERR_NODATA)
				break;
		}
	}
}

//
//void QtApp::local_files_changed(QStringList & strFiles)
//{
//	QString str;
//	for(int i=0;i<strFiles.size();++i)
//	{
//		str.append(strFiles[i]);
//		str.append("\n");
//		QList<PtrFile> rpf=sm->ls_file(strFiles[i]);//����URL
//		PtrFile pf=SyncBaseFile::getFileByUrl(strFiles[i]);
//		if(rpf[0]->getAnchor() < pf->getAnchor())
//		{
//			el->insertEvent(pf,SYNC_TO_SERVER,SYNC_OP_MODIFY);
//			sm->put_file(pf);
//		}
//		else
//		{
//			QString strInfo=rpf[0]->getUrl()+"�����ļ���ͻ������ê��"+QString::number(pf->getAnchor())+"�������ê��"+QString::number(rpf[0]->getAnchor())+"��һ��!";
//			QMessageBox(QMessageBox::NoIcon,"�ļ���ͻ��",strInfo).exec();
//		}
//	}
//	
//	if(str!="")
//		log("Changed:"+str);
//}
//void QtApp::local_files_added(QStringList & strFiles)
//{
//	QString str;
//	for(int i=0;i<strFiles.size();++i)
//	{
//		str.append(strFiles[i]);
//		str.append("\n");
//		//PtrFile pf=SyncBaseFile::getFileByUrl(strFiles[i]);
//		PtrFile pf=new SyncBaseFile();
//		pf->setLocalUrl(synconf->getstr(KEY_SYNCDIR)+strFiles[i]);
//		pf->setUrl(strFiles[i]);
//		el->insertEvent(pf,SYNC_TO_SERVER,SYNC_OP_ADD);
//		sm->put_file(pf);
//	}
//
//	if(str!="")
//		log("Added:"+str);
//}
//void QtApp::local_files_removed(QStringList & strFiles)
//{
//	QString str;
//	for(int i=0;i<strFiles.size();++i)
//	{
//		str.append(strFiles[i]);
//		str.append("\n");
//		PtrFile pf=SyncBaseFile::getFileByUrl(strFiles[i]);
//		el->insertEvent(pf,SYNC_TO_SERVER,SYNC_OP_REMOVE);
//		sm->rm_file(pf);
//	}
//
//	if(str!="")
//		log("Removed:"+str);
//}

void QtApp::recvAlertMessage(StringMap alert_props,AlertMessageList msglist)
{
	int syncdir=alert_props[KEY_SYNCDIR].toInt();
	if(syncdir != SYNC_TO_SERVER && syncdir!= SYNC_FROM_SERVER)
		return ;
	if(syncdir == SYNC_TO_SERVER)
	{
		for(int i=0;i<msglist.size();++i)
		{
			AlertMessage msg=msglist[i];
			PtrFile ptrfile=getFileObjectByUrl(msg[KEY_URL]);
			if(ptrfile.isNull())
				continue;
			switch(msg[KEY_ACTION_TYPE].toInt())
			{
			case SYNC_OP_ADD:
			case SYNC_OP_MODIFY:
				sm->put_file(ptrfile);
				break;
			case SYNC_OP_REMOVE:
				sm->rm_file(ptrfile);
				break;
			case SYNC_OP_RENAME:
				//sm->mv_file(ptrfile,msg[KEY_URL_NEW]);
				break;
			default:
				continue;
			}
		}
	}
	else
	{
		for(int i=0;i<msglist.size();++i)
		{
			AlertMessage msg=msglist[i];
			switch(msg[KEY_ACTION_TYPE].toInt())
			{
			case SYNC_OP_ADD:
			case SYNC_OP_MODIFY:
				sm->get_file(msg[KEY_URL]);
				break;
			case SYNC_OP_REMOVE:
				//rm local file
				break;
			case SYNC_OP_RENAME:
				break;
			default:
				continue;
			}
		}
	}
}



void QtApp::on_btnBrowseSyncdirectory_clicked()
{
	QString strDir=ui.txtSyncDirectory->text();
	if(strDir=="" || !QFileInfo(strDir).exists())
		strDir=synconf->getstr(KEY_SYNCDIR);
	strDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		strDir,
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	if(strDir!="" && QFileInfo(strDir).exists() && QFileInfo(strDir).isDir())
	{
		synconf->setstr(KEY_SYNCDIR,strDir);
		ui.txtSyncDirectory->setText(strDir);
	}
}


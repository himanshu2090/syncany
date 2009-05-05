#include "qtapp.h"
#include "syncbasefile.h"
#include "optiondlg.h"

QtApp::QtApp(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	bAllowClose=false;//阻止直接关闭窗口

	ui.setupUi(this);
	//ui.pushButton->hide();
	syncdb=SyncDB::instance();
	synconf=Synconf::instance();
	QString svrhost=synconf->getstr("server_host","5.152.69.23");
	QString svrport=synconf->getstr("server_port","18120");

	QString strUser=synconf->getstr("user_id");
	QString strPass=synconf->getstr("user_password");

	ui.lineEditUser->setText(strUser);
	ui.lineEditPassword->setText(strPass);

	synconf->setstr("server_host",svrhost);
	synconf->setstr("server_port",svrport,true);
	ui.textHost->setText(svrhost);
	ui.textPort->setText(svrport);
	sm=new SessionManager(this);
	watcher=new LocalFileWatcher(this);

	connect(watcher.data(),SIGNAL(filesChanged(QStringList &)),this,SLOT(local_files_changed(QStringList &)));
	connect(watcher.data(),SIGNAL(filesAdded(QStringList &)),this,SLOT(local_files_added(QStringList &)));
	connect(watcher.data(),SIGNAL(filesRemoved(QStringList &)),this,SLOT(local_files_removed(QStringList &)));
	
	createActions();
	createTrayIcon();

	connect(sm.data(),SIGNAL(msgBox(QString ,QString )),this,SLOT(trayMessage(QString ,QString)));

	trayMessage("SyncAny提示","双击显示窗口，单击激活窗口，右键菜单选择“退出”结束运行");
}

void QtApp::createActions()
{
	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), this, SLOT(trigger_quit()));
}

void QtApp::createTrayIcon()
{
	trayMenu = new QMenu(this);
	trayMenu->addSeparator();
	trayMenu->addAction(quitAction);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayMenu);
	QIcon icon=QIcon(".\\QtApp.ico");
	trayIcon->setIcon(icon);
	trayIcon->setToolTip("SyncAny Client Tray");
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
	synconf->setstr("server_host",ui.textHost->text());
	synconf->setstr("server_port",ui.textPort->text());
}

void QtApp::on_btnSend_clicked()
{
	QString strContent=ui.textInput->toPlainText();
	ui.textInput->setPlainText("");
	strContent.remove('\r');
	//为测试公开client
	sm->client.sendData(strContent+"\n");
}

void QtApp::on_btnClear_clicked()
{
	ui.textLogger->setPlainText("");
	ui.textLoggerIn->setPlainText("");
	ui.textLoggerOut->setPlainText("");
}

void QtApp::log(QString str)
{
	QMutexLocker autolocker(&m_locker_log);
	QString strFilename=synconf->getstr("work_dir")+"/"+QDateTime::currentDateTime().toString(QString("yyyy-MM-dd"))+".txt";
	QFile qf(strFilename);
	qf.open(QFile::WriteOnly);
	QString so=QDateTime::currentDateTime().toString(QString("yyyy-MM-dd hh:mm:ss : ")) +str.trimmed();
	qf.write(so.toLocal8Bit());
	qf.close();
	ui.textLogger->append(so);
}

void QtApp::login(QString str)
{
	QString so="<<:"+str.trimmed();
	ui.textLoggerIn->append(so);
}

void QtApp::logout(QString str)
{
	QString so=">>:"+str.trimmed();
	ui.textLoggerIn->append(so);
}

void QtApp::on_btnConnect_clicked()
{
	//应该更新HOST地址和端口
	sm->ConnectHost();
}

void QtApp::on_btnDisconnect_clicked()
{
	sm->DisconnectHost();
}

void QtApp::on_pushButton_2_clicked()
{

}

void QtApp::local_files_changed(QStringList & strFiles)
{
	QString str;
	for(int i=0;i<strFiles.size();++i)
	{
		str.append(strFiles[i]);
		str.append("\n");
		QList<PtrFile> rpf=sm->ls_file(strFiles[i]);//根据URL
		PtrFile pf=SyncBaseFile::getFileByUrl(strFiles[i]);
		if(rpf[0]->getAnchor() < pf->getAnchor())
			sm->put_file(pf);
		else
		{
			QString strInfo=rpf[0]->getUrl()+"出现文件冲突！本地锚点"+QString::number(pf->getAnchor())+"与服务器锚点"+QString::number(rpf[0]->getAnchor())+"不一致!";
			QMessageBox(QMessageBox::NoIcon,"文件冲突！",strInfo).exec();
		}
	}
	
	if(str!="")
		log("Changed:"+str);
}
void QtApp::local_files_added(QStringList & strFiles)
{
	QString str;
	for(int i=0;i<strFiles.size();++i)
	{
		str.append(strFiles[i]);
		str.append("\n");
		PtrFile pf=SyncBaseFile::getFileByUrl(strFiles[i]);
		sm->put_file(pf);
	}

	if(str!="")
		log("Added:"+str);
}
void QtApp::local_files_removed(QStringList & strFiles)
{
	QString str;
	for(int i=0;i<strFiles.size();++i)
	{
		str.append(strFiles[i]);
		str.append("\n");
		PtrFile pf=SyncBaseFile::getFileByUrl(strFiles[i]);
		sm->rm_file(pf);
	}

	if(str!="")
		log("Removed:"+str);
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
	QByteArray strurl="http://www.g.cn/?abc sdf中文!@#$%%^&&**(()__+";
	QByteArray s=raw_url_encode(strurl);
	qDebug(QString::fromLocal8Bit(strurl).toUtf8());
	QByteArray ss=raw_url_decode(s);
	qDebug(ss);
	qDebug("中文");

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
		//未注册
		synconf->setstr("session","");
		synconf->setstr("deviceid","");
		synconf->setstr("user_id",strUser);
		synconf->setstr("user_password",strPass,true);
	}
	else
	{
		if(QMessageBox::question(this,"更换帐号？","你已经在这台设备上注册了帐号"+synconf->getstr("user_id")+"，你确定要更换帐号吗？（更换帐号会抹除你的设备注册信息）",QMessageBox::Yes|QMessageBox::No,QMessageBox::No)== QMessageBox::Yes)
		{
			synconf->setstr("session","");
			synconf->setstr("deviceid","");
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
		synconf->setstr("session","");
		synconf->setstr("deviceid","");
		synconf->setstr("user_id",strUser);
		synconf->setstr("user_password",strPass);
		synconf->setstr("doregister","1",true);
	}
	else
	{
		if(QMessageBox::question(this,"重新注册？","你已经在这台设备上注册了帐号"+synconf->getstr("user_id")+"，你确定要重新注册吗？（重新注册会抹除你的设备注册信息）",QMessageBox::Yes|QMessageBox::No,QMessageBox::No)== QMessageBox::Yes)
		{
			synconf->setstr("session","");
			synconf->setstr("deviceid","");
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
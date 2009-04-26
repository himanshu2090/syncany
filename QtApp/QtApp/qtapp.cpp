#include "qtapp.h"
#include "syncbasefile.h"

#include <QMessageBox>

QtApp::QtApp(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{


	ui.setupUi(this);
	//ui.pushButton->hide();
	syncdb=SyncDB::instance();
	synconf=Synconf::instance();
	QString svrhost=synconf->getstr("server_host","5.152.69.23");
	QString svrport=synconf->getstr("server_port","18120");


	synconf->setstr("server_host",svrhost);
	synconf->setstr("server_port",svrport,true);
	ui.textHost->setText(svrhost);
	ui.textPort->setText(svrport);
	sm=new SessionManager(this);
	watcher=new LocalFileWatcher(this);

	connect(watcher.data(),SIGNAL(filesChanged(QStringList &)),this,SLOT(local_files_changed(QStringList &)));
	connect(watcher.data(),SIGNAL(filesAdded(QStringList &)),this,SLOT(local_files_added(QStringList &)));
	connect(watcher.data(),SIGNAL(filesRemoved(QStringList &)),this,SLOT(local_files_removed(QStringList &)));

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
	//Ϊ���Թ���client
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
	QString so="LOG:"+str.trimmed();
	qDebug(so.toLocal8Bit());
	qDebug("\n");
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
	ui.textLoggerOut->append(so);
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
		PtrFile rpf=sm->ls_file(strFiles[i]);//����URL
		PtrFile pf=SyncBaseFile::getFileByUrl(strFiles[i]);
		if(rpf->getAnchor() < pf->getAnchor())
			sm->put_file(pf);
		else
		{
			QString strInfo=rpf->getUrl()+"�����ļ���ͻ������ê��"+QString::number(pf->getAnchor())+"�������ê��"+QString::number(rpf->getAnchor())+"��һ��!";
			QMessageBox(QMessageBox::NoIcon,"�ļ���ͻ��",strInfo).exec();
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
		QByteArray strCmd="put "+sm->generate_cmdid().toLocal8Bit()+" url=/home/wujunping/testfold/"+ strFilename.toLocal8Bit() +" size="+QString::number(buf.length()).toLocal8Bit()+"\n"+buf;
		sm->client.sendData(strCmd);

	}
	else
	{
		QString strFilename="/home/wujunping/testfold/test.txt";
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
}
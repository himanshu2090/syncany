#include "syncbasefile.h"
/*
�ļ�ͬ����
filename	���ش����ļ�������ȫ·����
filesize	�����ļ���С
uri_local	�����URI��ʾ
anchor ê�㣨������������
anchor_time ê���Ӧ������޸�ʱ�䣬���ش洢���ڸ���ê��ʱ����
modify_time ����޸�ʱ��

CREATE TABLE [sync_files] (
[fid] AUTOINC, 
[url] VARCHAR NOT NULL, 
[filename] VARCHAR , 
[filesize] INT , 
[anchor] INT, 
[anchor_time] DATETIME, 
[modify_time] DATETIME,
CONSTRAINT [sqlite_autoindex_sync_files_uri] PRIMARY KEY ([url]));


*/
SyncBaseFile::SyncBaseFile(QObject *parent)
	: IFile(parent)
{
	syncdb=SyncDB::instance();
	pszTableName=strSyncTableName[SYNC_FILES];

	datasize=-1;
	url="";
	filename="";
	anchor=0;
}

SyncBaseFile::~SyncBaseFile()
{

}

void SyncBaseFile::createTable()
{
	//�����������Ҫ��ʼ�������ı�
	SyncDB *syncdb=SyncDB::instance();
	const char *pszTableName=strSyncTableName[SYNC_FILES];
	QString strTableName=pszTableName;
	if(!syncdb->tableExists(strTableName.toStdString().c_str()))
	{
		QString strSql;
		strSql+="CREATE TABLE [";
		strSql+=strTableName+"] (\n"
			"[fid] AUTOINC, \n"
			"[url] VARCHAR NOT NULL, \n"
			"[filename] VARCHAR ,\n" 
			"[filesize] INT , \n"
			"[anchor] INT, \n"
			"[anchor_time] DATETIME, \n"
			"[modify_time] DATETIME,\n"
			"CONSTRAINT [sqlite_autoindex_"+strTableName+"_uri] PRIMARY KEY ([url]));\n";
		syncdb->execSql(strSql);
	}
}

QDateTime SyncBaseFile::getLastModifyTime() //����޸�ʱ��
{
	if(lastmodify.isValid())
		return lastmodify;
	return QDateTime();
}

quint32 SyncBaseFile::setLastModifyTime(QDateTime dt)
{
	lastmodify=dt;
	return 0;
}

QDateTime SyncBaseFile::getAnchorTime()
{
	if(anchortime.isValid())
		return anchortime;
	return QDateTime();
}

quint32 SyncBaseFile::setAnchorTime(QDateTime dt)
{
	anchortime=dt;
	return 0;
}

quint32 SyncBaseFile::getAnchor() //ê����Ϣ
{
	return anchor;
}
quint32 SyncBaseFile::setAnchor(quint32 anch)//����ê��
{
	if(anch>0)
	{
		anchor=anch;
		return anchor;
	}
	return 0;
}
quint32 SyncBaseFile::getSize() //��ȡ���ݴ�С
{
	return datasize;
}

quint32 SyncBaseFile::setSize(quint32 sz) //�������ݴ�С
{
	datasize=sz;
	return datasize;
}

QByteArray SyncBaseFile::getData()//��ȡ����
{
	if(filename=="")
		return QByteArray();
	QFile qf(filename);
	qf.open(QIODevice::ReadOnly);
	QByteArray buf=qf.readAll();
	qf.close();
	return buf;
}
quint32 SyncBaseFile::setData(QByteArray buf)//��������
{
	if(filename=="")
		return 0;
	QFile qf(filename);
	qf.open(QIODevice::Truncate);
	quint32 ret=qf.write(buf);
	qf.close();
	return ret;
}
QString SyncBaseFile::getUrl()//��ȡ·����Ϣ
{
	return url;
}

quint32 SyncBaseFile::setUrl(QString strUrl)
{
	url=strUrl;
	return 0;
}
QString SyncBaseFile::getLocalUrl()//��ȡlocalfile��Ϣ
{
	return filename;
}

quint32 SyncBaseFile::setLocalUrl(QString strUrl)//����localfile��Ϣ
{
	filename=strUrl;
	return 0;
}

quint32 SyncBaseFile::flush()
{
	if(!SyncBaseFile::getFileByUrl(url).isNull())
	{
		QString strSql="update sync_files set url='"+url+"'";
		if(filename!="")
			strSql+=",filename='"+filename+"',filesize="+QString::number(datasize);
		if(anchor>0)
			strSql+=",anchor="+QString::number(anchor);
		if(anchortime.isValid())
			strSql+=",anchor_time='"+anchortime.toString(Qt::ISODate)+"'";
		if(lastmodify.isValid())
			strSql+=",modify_time='"+lastmodify.toString(Qt::ISODate)+"'";
		strSql+=" where url='"+url+"'";
		return syncdb->execSql(strSql);
	}
	else
	{
		QString strField="(url";
		QString strValue="('"+url+"'";
		if(filename!="")
		{
			strField+=",filename,filesize";
			strValue+=",'"+filename+"',"+QString::number(datasize);
		}
		if(anchor>0)
		{
			strField+=",anchor";
			strValue+=","+QString::number(anchor);
		}
		if(anchortime.isValid())
		{
			strField+=",anchor_time";
			strValue+=",'"+anchortime.toString(Qt::ISODate)+"'";
		}
		if(lastmodify.isValid())
		{
			strField+=",modify_time";
			strValue+=",'"+lastmodify.toString(Qt::ISODate)+"'";
		}
		strField+=")";
		strValue+=")";
		QString strSql="insert into sync_files "+ strField+" values "+strValue;
		return syncdb->execSql(strSql);
	}
}

PtrFile SyncBaseFile::getFileByUrl(QString strUrl)
{
	QString strSql="select * from sync_files where url='"+strUrl+"' limit 1";
	CommandMap rs=SyncDB::instance()->singleQuerySql(strSql);
	if(rs.size()==0)
		return PtrFile();
	PtrFile pf=new SyncBaseFile(null);
	pf->setUrl(strUrl);
	pf->setLocalUrl(rs["filename"]);
	quint32 i=rs["anchor"].toInt();
	if(i>0)
		pf->setAnchor(i);
	i=rs["filesize"].toInt();
	if(i>0)
		pf->setSize(i);
	if(rs["anchor_time"]!="")
		pf->setAnchorTime(QDateTime::fromString(rs["anchor_time"],Qt::ISODate));
	if(rs["modify_time"]!="")
		pf->setLastModifyTime(QDateTime::fromString(rs["modify_time"],Qt::ISODate));
	return pf;
}

PtrFile SyncBaseFile::getFileByFileName(QString strFilename)
{
	QString strSql="select * from sync_files where filename='"+strFilename+"' limit 1";
	CommandMap rs=SyncDB::instance()->singleQuerySql(strSql);
	if(rs.size()==0)
		return PtrFile();
	PtrFile pf=new SyncBaseFile(null);
	pf->setUrl(strFilename);
	pf->setLocalUrl(rs["filename"]);
	quint32 i=rs["anchor"].toInt();
	if(i>0)
		pf->setAnchor(i);
	i=rs["filesize"].toInt();
	if(i>0)
		pf->setSize(i);
	if(rs["anchor_time"]!="")
		pf->setAnchorTime(QDateTime::fromString(rs["anchor_time"],Qt::ISODate));
	if(rs["modify_time"]!="")
		pf->setLastModifyTime(QDateTime::fromString(rs["modify_time"],Qt::ISODate));
	return pf;
}
 //��ȡ���ݿ����ȫ������
QMap<QString,PtrFile> SyncBaseFile::getAllFiles()
{
	QString strSql="select * from sync_files";
	QMap<QString,PtrFile> ret;
	CppSQLite3Query rs=SyncDB::instance()->querySql(strSql);
	while(!rs.eof())
	{
		CommandMap props;
		for(int i=0;i<rs.numFields();++i)
		{
			props[rs.fieldName(i)]=rs.fieldValue(i);
		}
		PtrFile pf=new SyncBaseFile(null);
		pf->setUrl(props["url"]);
		pf->setLocalUrl(props["filename"]);
		quint32 i=props["anchor"].toInt();
		if(i>0)
			pf->setAnchor(i);
		i=props["filesize"].toInt();
		if(i>0)
			pf->setSize(i);
		if(props["anchor_time"]!="")
			pf->setAnchorTime(QDateTime::fromString(props["anchor_time"],Qt::ISODate));
		if(props["modify_time"]!="")
			pf->setLastModifyTime(QDateTime::fromString(props["modify_time"],Qt::ISODate));

		ret[props["url"]]=pf;
		rs.nextRow();
	}
	return ret;
}


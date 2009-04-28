#ifndef SYNCBASEDIR_H
#define SYNCBASEDIR_H

#include "common.h"
#include "synconf.h"

class SyncBaseDir : public IFolder
{
	Q_OBJECT

public:
	SyncBaseDir(QObject *parent=null);
	~SyncBaseDir();

	//File virtual function
	virtual QByteArray getData();//获取内容
	virtual QDateTime getLastModifyTime(); //最后修改时间
	virtual QDateTime getAnchorTime();
	virtual quint32 getAnchor(); //锚点信息
	virtual quint32 getSize(); //内容大小
	virtual QString getUrl();//获取路径信息
	virtual QString getLocalUrl();//获取localfile信息

	virtual quint32 getType();//获取类型（文件，目录或者。。。？）

	virtual quint32 setData(QByteArray);//设置内容
	virtual quint32 setLastModifyTime(QDateTime); //最后修改时间
	virtual quint32 setAnchor(quint32);//设置锚点
	virtual quint32 setAnchorTime(QDateTime dt);//设置锚点对应的时间信息
	virtual quint32 setUrl(QString strUrl);//设置URI信息
	virtual quint32 setLocalUrl(QString strUrl);//设置localfile信息
	virtual quint32 setSize(quint32);//设置大小值

	virtual quint32 flush();//将变更记录下来

	//Folder virtual function

	virtual quint32 insert(PtrFile subfile);
	virtual quint32 remove(PtrFile subfile);
	virtual QList<PtrFile> getSubFiles();

private:
	Synconf *synconf;
	QMap<QString ,PtrFile> subfiles;
	QDateTime modify_time;
	QString strUrl;
	QString strLocalUrl;
};

#endif // SYNCBASEDIR_H

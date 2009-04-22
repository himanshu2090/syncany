#ifndef SYNCBASEFILE_H
#define SYNCBASEFILE_H

#include "common.h"

class SyncBaseFile : public IFile
{
	Q_OBJECT

public:
	SyncBaseFile(QObject *parent);
	~SyncBaseFile();
public:
	virtual QDateTime getLastModifyTime(); //最后修改时间
	virtual QDateTime getAnchorTime();
	virtual quint32 getAnchor(); //锚点信息
	virtual quint32 setAnchor(quint32);//设置锚点
	virtual quint32 size(); //内容大小
	virtual QByteArray getData();//获取内容
	virtual quint32 setData(QByteArray);//设置内容
	virtual QString getUri();//获取路径信息

private:
	
};

#endif // SYNCBASEFILE_H

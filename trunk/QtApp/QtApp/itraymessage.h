#ifndef ITRAYMESSAGE_H
#define ITRAYMESSAGE_H

#include "common.h"

class ITrayMessage : public QObject
{
	Q_OBJECT

public:
	ITrayMessage(QObject *parent=null);
	~ITrayMessage();
signals:
	void trayMessage(QString strTitle,QString strInfo);
	void trayLog(QString strInfo);
	//本地文件变化通知
	void filesChanged(QStringList &strFiles);
	void filesAdded(QStringList &strFiles);
	void filesRemoved(QStringList &strFiles);
	void filesRenamed(QStringList &strFiles,QStringList &strNewFiles);

	//远程Alert事件
private:
	
};

#endif // ITRAYMESSAGE_H

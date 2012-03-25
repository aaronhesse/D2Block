#pragma once

#include <QObject>

class D2BlockDownloader : public QObject
{
	Q_OBJECT

private:
	QNetworkAccessManager* netManager;


public slots:
	void DownloadFile(const QString url, QByteArray* replyData, qint64 dataSize);

public:
	D2BlockDownloader(void);
	~D2BlockDownloader(void);

};


#pragma once

#include <QObject>

class D2BlockDownloader : public QObject
{
	Q_OBJECT

public:
	D2BlockDownloader(void);
	~D2BlockDownloader(void);

	QByteArray DownloadFile(const QString& url);

public slots:
	void on_downloadFinished(QNetworkReply* data);

private:
	QNetworkAccessManager* m_netManager;
	QByteArray m_replyData;
};


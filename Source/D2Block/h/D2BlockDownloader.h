#pragma once

#include <QObject>

class D2BlockDownloader : public QObject
{
	Q_OBJECT

public:

	D2BlockDownloader(void);
	~D2BlockDownloader(void);

	QByteArray DownloadFile(const QString& url);
	bool DownloadFileToDisk(const QString& url, const QString& pathOnDisk);

public slots:

	void on_downloadFinished(QNetworkReply* data);

private:

	QNetworkAccessManager m_netManager;
	QByteArray m_replyData;
};


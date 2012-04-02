#include "StdAfx.h"
#include "D2BlockDownloader.h"


D2BlockDownloader::D2BlockDownloader(void)
{
	m_netManager = new QNetworkAccessManager(this);

	QObject::connect(m_netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_downloadFinished(QNetworkReply*)));
}

D2BlockDownloader::~D2BlockDownloader(void)
{
	delete m_netManager;
	m_netManager = nullptr;
}

bool D2BlockDownloader::DownloadFileToDisk(const QString& url, const QString& pathOnDisk)
{
	bool retVal = false;

	QByteArray fileData = DownloadFile(url);

	QFile* file = new QFile(pathOnDisk);

	if (file->open(QIODevice::WriteOnly))
	{
		if (file->write(fileData))
			retVal = true;

		file->close();
	}

	delete file;
	file = nullptr;

	return retVal;
}

QByteArray D2BlockDownloader::DownloadFile(const QString& url)
{
	QNetworkRequest request(url);
	QNetworkReply* reply = m_netManager->get(request);

	// Block until the reply/download is complete.
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	delete reply;

	return m_replyData;
}

void D2BlockDownloader::on_downloadFinished(QNetworkReply* data)
{
	m_replyData = data->readAll();
}

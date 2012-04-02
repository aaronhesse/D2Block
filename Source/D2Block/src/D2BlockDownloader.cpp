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

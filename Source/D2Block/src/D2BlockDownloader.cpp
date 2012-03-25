#include "StdAfx.h"
#include "D2BlockDownloader.h"


D2BlockDownloader::D2BlockDownloader(void)
{
	netManager = new QNetworkAccessManager(this);
}


D2BlockDownloader::~D2BlockDownloader(void)
{
	delete netManager;
	netManager = nullptr;
}

void D2BlockDownloader::DownloadFile(const QString url, QByteArray* replyData, qint64 dataSize)
{	
	QNetworkReply* reply = netManager->get(QNetworkRequest(QUrl(url)));

	// Block until the reply/download is complete.
	while(!reply->isFinished())
		Sleep(1);

	replyData = &reply->readAll(); // if there's a problem with this, perform a memcpy. we don't know the lifetime of this data.
	dataSize = reply->readBufferSize();
}

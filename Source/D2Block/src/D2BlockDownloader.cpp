#include "StdAfx.h"
#include "D2BlockDownloader.h"

D2BlockDownloader::D2BlockDownloader():
    m_netManager(this)
{
    QObject::connect(&m_netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_downloadFinished(QNetworkReply*)));
}

D2BlockDownloader::~D2BlockDownloader()
{
}

bool D2BlockDownloader::DownloadFileToDisk(const QString& url, const QString& pathOnDisk)
{
    bool retVal = false;

    const QByteArray fileData = DownloadFile(url);

    QFile file(pathOnDisk);

    if (file.open(QIODevice::WriteOnly))
    {
        if (file.write(fileData))
            retVal = true;

        file.close();
    }

    return retVal;
}

QByteArray D2BlockDownloader::DownloadFile(const QString& url)
{
    QNetworkRequest request(url);
    QNetworkReply* reply = m_netManager.get(request);

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

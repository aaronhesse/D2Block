#include "StdAfx.h"
#include "D2BlockUpdater.h"
#include "D2BlockDownloader.h"

D2BlockUpdater::D2BlockUpdater():
m_localRevision(0),
m_ignoreListOutOfDate(false)
{
}

D2BlockUpdater::~D2BlockUpdater()
{
}

void D2BlockUpdater::UpdateIgnoreList()
{
	if (IgnoreListIsOutOfDate())
		UpdateIgnoreListFile();

	emit UpdaterComplete();
}

void D2BlockUpdater::ProcessRegistryInformation()
{
	QSettings settings("D2Block", "D2Block");

	m_httpServer = settings.value("Server").toString();
 	m_updateFile = settings.value("Update File").toString();
 	m_ignorelistFile = settings.value("Ignorelist File").toString();
 	m_localRevision = settings.value("Current Revision").toInt();
}

void D2BlockUpdater::ProcessVersionFile()
{
	m_url = QString("http://%1/%2").arg(m_httpServer).arg(m_updateFile);

	D2BlockDownloader downloader;
	QByteArray fileData = downloader.DownloadFile(m_url);

	if (atoi(fileData) > m_localRevision)
		m_ignoreListOutOfDate = true;
}

bool D2BlockUpdater::IgnoreListIsOutOfDate()
{
	ProcessRegistryInformation();
	ProcessVersionFile();

	return m_ignoreListOutOfDate;
}

void D2BlockUpdater::UpdateIgnoreListFile()
{
	if (DownloadUpdatedIgnoreListFile())
	{
		BackupIgnoreListFile();
		MergeIgnoreLists();
		UpdateRevisionNumber();
		m_ignoreListOutOfDate = false;
	}

	m_ignoreListOutOfDate = true;
}

bool D2BlockUpdater::DownloadUpdatedIgnoreListFile()
{
	// download Ignorelist from web server to d2root/ignorelist.update
	// If we fail to download it, just silently fail. We don't want
	// any of the UI to block or be annoying.

	QString url = QString("http://%1/%2").arg(m_httpServer).arg(m_ignorelistFile);

	D2BlockDownloader downloader;
	QByteArray fileData = downloader.DownloadFile(url);

	QString ignorelistUpdateFilePath = m_gamePath + "/" + "ignorelist.update";

	QFile* file = new QFile(m_updateFile);
	if (!file->open(QIODevice::WriteOnly))
	{
		delete file;
		file = nullptr;
		return false;
	}

	file->write(fileData);
	file->close();

	return true;
}

void D2BlockUpdater::BackupIgnoreListFile()
{
	// copy user ignorelist to 'ignorelist.bak'
}

void D2BlockUpdater::MergeIgnoreLists()
{
	// open ignorelist.bak
	// parse file for "!**D2BLOCK BEGIN**" header
	// save any lines above this.
	// parse file for "!**D2BLOCK END**" footer
	// save any lines below this.
	// append any saved lines to the end of ignorelist.update
	// rename ignorelist.update to 'ignorelist'
}

void D2BlockUpdater::UpdateRevisionNumber()
{
	// if everything is successful, set regkey for current revision
}
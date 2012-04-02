#include "StdAfx.h"
#include "D2BlockUpdater.h"
#include "D2BlockDownloader.h"

D2BlockUpdater::D2BlockUpdater():
m_localRevision(0),
m_remoteRevision(0),
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
	m_remoteRevision = atoi(fileData);

	if (m_remoteRevision > m_localRevision)
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

bool D2BlockUpdater::DownloadUpdatedIgnoreListFile() const
{
	QString url = QString("http://%1/%2").arg(m_httpServer).arg(m_ignorelistFile);
	QString ignorelistUpdateFilePath = m_gamePath + "/" + "ignorelist.update";

	D2BlockDownloader downloader;
	return downloader.DownloadFileToDisk(url, ignorelistUpdateFilePath);
}

void D2BlockUpdater::BackupIgnoreListFile() const
{
	QFile::rename(m_gamePath + "ignorelist", m_gamePath + "ignorelist.bak");
}

void D2BlockUpdater::MergeIgnoreLists() const
{
	// open ignorelist.bak
	// parse file for "!**D2BLOCK BEGIN**" header
	// save any lines above this.
	// parse file for "!**D2BLOCK END**" footer
	// save any lines below this.
	// append any saved lines to the end of ignorelist.update
    QFile::rename(m_gamePath + "ignorelist.update", m_gamePath + "/" + "ignorelist");
}

void D2BlockUpdater::UpdateRevisionNumber() const
{
	QSettings settings;
	settings.setValue("Revision", m_remoteRevision);
}
#include "StdAfx.h"
#include "D2BlockUpdater.h"
#include "D2BlockDownloader.h"

const QString D2BlockUpdater::m_ignorelistBakFile = "ignorelist.bak";
const QString D2BlockUpdater::m_ignorelistUpdatedFile = "ignorelist.updated";

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
 	m_localRevision = settings.value("Local Revision").toInt();

	m_gamePath = QSettings("Blizzard Entertainment", "Diablo II").value("InstallPath").toString();

	emit setIgnoreListPathOnWindow(m_gamePath + m_ignorelistFile);
	emit updateProgressBar(20);
}

void D2BlockUpdater::ProcessVersionFile()
{
	QString url = QString("http://%1/%2").arg(m_httpServer).arg(m_updateFile);

	D2BlockDownloader downloader;
	QByteArray fileData = downloader.DownloadFile(url);
	m_remoteRevision = atoi(fileData);

	if (m_remoteRevision > m_localRevision)
		m_ignoreListOutOfDate = true;

	emit updateProgressBar(20);
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
		if (MergeIgnoreLists())
		{
			UpdateRevisionNumber();
			m_ignoreListOutOfDate = false;
			return;
		}
	}

	Cleanup();
	m_ignoreListOutOfDate = true;
}

bool D2BlockUpdater::DownloadUpdatedIgnoreListFile() const
{
	QString url = QString("http://%1/%2").arg(m_httpServer).arg(m_ignorelistFile);
	QString ignoreListUpdateFilePath = m_gamePath + m_ignorelistUpdatedFile;

	D2BlockDownloader downloader;
	bool success = downloader.DownloadFileToDisk(url, ignoreListUpdateFilePath);
	emit updateProgressBar(30);
	return success;
}

void D2BlockUpdater::BackupIgnoreListFile() const
{
	QFile::copy(m_gamePath + m_ignorelistFile, m_gamePath + m_ignorelistBakFile);
}

bool D2BlockUpdater::MergeIgnoreLists() const
{
	QFile bakFile(m_gamePath + m_ignorelistBakFile);
	if(!bakFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	// Look for the sections of the file to ignore. Grab all of the other lines.

	bool ignoreLines = false;
	QStringList userIgnoreListData;

	while (!bakFile.atEnd()) {
		QByteArray line = bakFile.readLine();

		if (line.contains("!**D2BLOCK BEGIN**"))
			ignoreLines = true;
		else if (line.contains("!**D2BLOCK END**"))
		{
			ignoreLines = false;
			continue;
		}

		if (!ignoreLines)
			userIgnoreListData.push_back(line);
	}

	bakFile.close();

	// Then add all the user sections of the file to the updated file.
	QFile updateFile(m_gamePath + m_ignorelistUpdatedFile);
	if(!updateFile.open(QIODevice::Append | QIODevice::Text))
		return false;

	foreach(QString entry, userIgnoreListData)
	{
		updateFile.write(entry.toAscii());
	}

	updateFile.close();

	QFile::remove(m_gamePath + m_ignorelistFile);
	QFile::remove(m_gamePath + m_ignorelistBakFile);
	QFile::rename(m_gamePath + m_ignorelistUpdatedFile, m_gamePath + m_ignorelistFile);

	emit updateProgressBar(20);

	return true;
}

void D2BlockUpdater::UpdateRevisionNumber() const
{
	QSettings(QSettings::SystemScope, "D2Block", "D2Block").setValue("Local Revision", m_remoteRevision);
}

void D2BlockUpdater::Cleanup() const
{
	QFile::remove(m_gamePath + m_ignorelistUpdatedFile);
	QFile::remove(m_gamePath + m_ignorelistBakFile);
}
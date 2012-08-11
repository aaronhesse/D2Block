#include "StdAfx.h"
#include "D2BlockUpdater.h"
#include "D2BlockDownloader.h"
#include "D2BlockSettings.h"

D2BlockUpdater::D2BlockUpdater():
    m_ignorelistBakFile("ignorelist.bak"),
    m_ignorelistUpdatedFile("ignorelist.updated"),
    m_d2blockStartBlock("!**D2BLOCK BEGIN**"),
    m_d2blockEndBlock("!**D2BLOCK END**"),
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
    ProcessRegistryInformation();

    if (!IsDiablo2Installed())
    {
        emit diablo2NotInstalled();
    }
    else if (IgnoreListIsOutOfDate())
    {
        UpdateIgnoreListFile();
        emit updaterComplete();
    }
}

bool D2BlockUpdater::IsDiablo2Installed()
{
    return (m_gamePath.isEmpty()) ? false : true;
}

void D2BlockUpdater::ProcessRegistryInformation()
{
    D2BlockSettings settings;

    m_httpServer     = settings.Server();
    m_updateFile     = settings.RevisionFile();
    m_ignorelistFile = settings.IgnorelistFile();
    m_localRevision  = settings.LocalRevision();
    m_gamePath       = settings.GameInstallPath();

#if defined(Q_OS_WIN)
    // If the ini file doesn't contain the game's installPath, then read it from the registry.
    if (m_gamePath.isEmpty())
        m_gamePath = QSettings("Blizzard Entertainment", "Diablo II").value("InstallPath").toString();
#endif

    emit setFilePathText(m_gamePath + m_ignorelistFile);
    emit updateProgressBar(20);
}

void D2BlockUpdater::on_setGameInstallPath(const QString& installPath)
{
    m_gamePath = installPath;

    D2BlockSettings settings;
    settings.setGameInstallPath(m_gamePath);
}

void D2BlockUpdater::ProcessVersionFile()
{
    const QString url = QString("http://%1/%2").arg(m_httpServer).arg(m_updateFile);
    const QByteArray fileData = D2BlockDownloader().DownloadFile(url);

    m_remoteRevision = atoi(fileData);

    if (m_remoteRevision > m_localRevision)
        m_ignoreListOutOfDate = true;

    emit updateProgressBar(20);
}

bool D2BlockUpdater::IgnoreListIsOutOfDate()
{
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

bool D2BlockUpdater::DownloadUpdatedIgnoreListFile()
{
    const QString url = QString("http://%1/%2").arg(m_httpServer).arg(m_ignorelistFile);
    const QString ignoreListUpdateFilePath = m_gamePath + m_ignorelistUpdatedFile;

    const bool success = D2BlockDownloader().DownloadFileToDisk(url, ignoreListUpdateFilePath);

    emit updateProgressBar(30);
    return success;
}

void D2BlockUpdater::BackupIgnoreListFile() const
{
    QFile::copy(m_gamePath + m_ignorelistFile, m_gamePath + m_ignorelistBakFile);
}

bool D2BlockUpdater::MergeIgnoreLists()
{
    QStringList userIgnoreListData;

    QFile bakFile(m_gamePath + m_ignorelistBakFile);
    if (bakFile.exists())
    {
        if (!bakFile.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;

        // Look for the sections of the file to ignore. Grab all of the other lines.

        bool ignoreLines = false;

        while (!bakFile.atEnd()) {
            QByteArray line = bakFile.readLine();

            if (line.contains(m_d2blockStartBlock.toAscii()))
                ignoreLines = true;
            else if (line.contains(m_d2blockEndBlock.toAscii()))
            {
                ignoreLines = false;
                continue;
            }

            if (!ignoreLines)
                userIgnoreListData.push_back(line);
        }

        bakFile.close();
    }

    // Then add all the user sections of the file to the updated file.
    QFile updateFile(m_gamePath + m_ignorelistUpdatedFile);
    if (!updateFile.open(QIODevice::Append | QIODevice::Text))
        return false;

    foreach (QString entry, userIgnoreListData)
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
    D2BlockSettings().setLocalRevision(m_remoteRevision);
}

void D2BlockUpdater::Cleanup() const
{
    QFile::remove(m_gamePath + m_ignorelistUpdatedFile);
    QFile::remove(m_gamePath + m_ignorelistBakFile);
}

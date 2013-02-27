#include "StdAfx.h"
#include "D2BlockUpdater.h"
#include "D2BlockDownloader.h"
#include "D2BlockSettings.h"

D2BlockUpdater::D2BlockUpdater():
    m_ignorelistBakFile("ignorelist.bak"),
    m_ignorelistUpdatedFile("ignorelist.updated"),
    m_d2blockStartBlock("!**D2BLOCK BEGIN**"),
    m_d2blockEndBlock("!**D2BLOCK END**"),
    m_remoteRevision(0),
    m_ignoreListOutOfDate(false)
{
}

D2BlockUpdater::~D2BlockUpdater()
{
}

void D2BlockUpdater::UpdateIgnoreList()
{
    emit setProgressTitle(tr("Updating Diablo II Ignorelist..."));
    emit showProgressBar();

    ProcessRegistryInformation();

    if (!IsDiablo2Installed())
    {
        emit diablo2NotInstalled();
        return;
    }
    else if (IgnoreListIsOutOfDate())
    {
        UpdateIgnoreListFile();
    }

    emit updaterComplete();
}

bool D2BlockUpdater::IsDiablo2Installed()
{
    return (D2BlockSettings::getInstance()->GameInstallPath().isEmpty()) ? false : true;
}

void D2BlockUpdater::ProcessRegistryInformation()
{
    D2BlockSettings* settings = D2BlockSettings::getInstance();

#if defined(Q_OS_WIN)

    if (settings->GameInstallPath().isEmpty())
        settings->setGameInstallPath(QSettings("Blizzard Entertainment", "Diablo II").value("InstallPath").toString());

#endif

    emit setFilePathText(settings->GameInstallPath() + settings->IgnorelistFile());
    emit updateProgressBar(20);
}

void D2BlockUpdater::on_setGameInstallPath(const QString& installPath)
{
    D2BlockSettings* settings = D2BlockSettings::getInstance();
    settings->setGameInstallPath(installPath);

#if defined(Q_OS_MAC)

    // QSettings objects don't properly support reading .prefs files, which Diablo II uses.
    // So we have to manually set the launchTarget here now that we know the installPath.

    settings->setLaunchTarget(QString(settings->GameInstallPath() + "Diablo II (Carbon)"));

#endif

    UpdateIgnoreList();
}

void D2BlockUpdater::ProcessVersionFile()
{
    const D2BlockSettings* settings = D2BlockSettings::getInstance();

    const QString url = QString("http://%1/%2").arg(settings->Server()).arg(settings->RevisionFile());
    const QByteArray fileData = D2BlockDownloader().DownloadFile(url);

    m_remoteRevision = atoi(fileData);

    if (m_remoteRevision > settings->LocalRevision())
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
    const D2BlockSettings* settings = D2BlockSettings::getInstance();

    const QString url = QString("http://%1/%2").arg(settings->Server()).arg(settings->IgnorelistFile());
    const QString ignoreListUpdateFilePath = settings->GameInstallPath() + m_ignorelistUpdatedFile;

    const bool success = D2BlockDownloader().DownloadFileToDisk(url, ignoreListUpdateFilePath);

    emit updateProgressBar(30);
    return success;
}

void D2BlockUpdater::BackupIgnoreListFile() const
{
    const D2BlockSettings* settings = D2BlockSettings::getInstance();
    const QString gamePath = settings->GameInstallPath();

    QFile::copy(gamePath + settings->IgnorelistFile(), gamePath + m_ignorelistBakFile);
}

bool D2BlockUpdater::MergeIgnoreLists()
{
    QStringList userIgnoreListData;

    const D2BlockSettings* settings = D2BlockSettings::getInstance();
    const QString gamePath = settings->GameInstallPath();

    QFile bakFile(gamePath + m_ignorelistBakFile);
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
    QFile updateFile(gamePath + m_ignorelistUpdatedFile);
    if (!updateFile.open(QIODevice::Append | QIODevice::Text))
        return false;

    foreach (QString entry, userIgnoreListData)
        updateFile.write(entry.toAscii());

    updateFile.close();

    QFile::remove(gamePath + settings->IgnorelistFile());
    QFile::remove(gamePath + m_ignorelistBakFile);
    QFile::rename(gamePath + m_ignorelistUpdatedFile, gamePath + settings->IgnorelistFile());

    emit updateProgressBar(20);

    return true;
}

void D2BlockUpdater::UpdateRevisionNumber() const
{
    D2BlockSettings::getInstance()->setLocalRevision(m_remoteRevision);
}

void D2BlockUpdater::Cleanup() const
{
    const QString gamePath = D2BlockSettings::getInstance()->GameInstallPath();

    QFile::remove(gamePath + m_ignorelistUpdatedFile);
    QFile::remove(gamePath + m_ignorelistBakFile);
}

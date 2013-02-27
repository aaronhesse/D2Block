#pragma once

#include <QObject>
#include "ui_d2block.h"

class D2BlockUpdater : public QObject
{
    Q_OBJECT

public:

    D2BlockUpdater();
    ~D2BlockUpdater();

    void UpdateIgnoreList();

private:

    const QString m_ignorelistBakFile;
    const QString m_ignorelistUpdatedFile;
    const QString m_d2blockStartBlock;
    const QString m_d2blockEndBlock;

    qint32 m_remoteRevision;

    bool m_ignoreListOutOfDate;

    void ProcessRegistryInformation();
    void ProcessVersionFile();
    void UpdateIgnoreListFile();
    bool IgnoreListIsOutOfDate();
    bool DownloadUpdatedIgnoreListFile();
    bool MergeIgnoreLists();
    bool IsDiablo2Installed();

    void BackupIgnoreListFile() const;
    void UpdateRevisionNumber() const;
    void Cleanup() const;

signals:

    void diablo2NotInstalled();
    void updaterComplete();
    void updateProgressBar(const qint32&);
    void setFilePathText(const QString&);
    void setProgressTitle(const QString&);
    void showProgressBar();

public slots:

    void on_setGameInstallPath(const QString&);
};

#pragma once

#include <QObject>

class D2BlockUpdater : public QObject
{
	Q_OBJECT

public:

	D2BlockUpdater();
	~D2BlockUpdater();

public slots:
	void UpdateIgnoreList();

private:

	QString m_gamePath;
	QString m_httpServer;
	QString m_updateFile;
	QString m_ignorelistFile;
	
	const QString m_iniFilePath;
	const QString m_ignorelistBakFile;
	const QString m_ignorelistUpdatedFile;

	qint32 m_localRevision;
	qint32 m_remoteRevision;

	bool m_ignoreListOutOfDate;

	void ProcessRegistryInformation();
	void ProcessVersionFile();
	bool IgnoreListIsOutOfDate();
	void UpdateIgnoreListFile();
	bool DownloadUpdatedIgnoreListFile();
	bool MergeIgnoreLists();

	void BackupIgnoreListFile() const;
	void UpdateRevisionNumber() const;
	void Cleanup() const;

signals:
	void UpdaterComplete();
	void updateProgressBar(qint32 amount);
	void setFilePathText(QString path);
};
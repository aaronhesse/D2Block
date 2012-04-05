#pragma once

#include <QObject>

class D2BlockUpdater : public QObject
{
	Q_OBJECT

public:

	D2BlockUpdater();
	~D2BlockUpdater();

	void UpdateIgnoreList();

private:

	QString m_gamePath;
	QString m_httpServer;
	QString m_updateFile;
	QString m_ignorelistFile;

	const QString m_ignorelistBakFile;
	const QString m_ignorelistUpdatedFile;
	const QString m_d2blockStartBlock;
	const QString m_d2blockEndBlock;

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

	void updaterComplete();
	void updateProgressBar(const qint32& amount);
	void setFilePathText(const QString& path);
};
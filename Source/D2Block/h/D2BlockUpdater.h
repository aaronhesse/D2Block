#include <QObject>
#include <QFile>

const size_t maxFilenameLength = 32;
const size_t maxWebserverAddressLength = 64;
const size_t maxUrlLength = maxWebserverAddressLength + maxFilenameLength;

class D2BlockUpdater : public QObject
{
	Q_OBJECT

private:

	QString m_url;
	QString m_httpServer;
	QString m_updateFile;
	QString m_ignorelistFile;
	QString m_gamePath;

	qint32 m_localRevision;
	qint32 m_remoteRevision;

	bool m_ignoreListOutOfDate;

	void ProcessRegistryInformation();
	void ProcessVersionFile();
	bool IgnoreListIsOutOfDate();
	void UpdateIgnoreListFile();
	bool DownloadUpdatedIgnoreListFile() const;
	void BackupIgnoreListFile() const;
	void MergeIgnoreLists() const;
	void UpdateRevisionNumber() const;

signals:
	void UpdaterComplete();

public slots:
	void UpdateIgnoreList();

public:

	D2BlockUpdater();
	~D2BlockUpdater();
};
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

	bool m_ignoreListOutOfDate;

	void ProcessRegistryInformation();
	void ProcessVersionFile();
	bool IgnoreListIsOutOfDate();
	void UpdateIgnoreListFile();
	bool DownloadUpdatedIgnoreListFile();
	void BackupIgnoreListFile();
	void MergeIgnoreLists();
	void UpdateRevisionNumber();

signals:
	void UpdaterComplete();

public slots:
	void UpdateIgnoreList();

public:

	D2BlockUpdater();
	~D2BlockUpdater();
};
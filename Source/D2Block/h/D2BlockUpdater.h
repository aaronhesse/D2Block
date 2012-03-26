#include <QObject>
#include <QFile>

const size_t maxFilenameLength = 32;
const size_t maxWebserverAddressLength = 64;
const size_t maxUrlLength = maxWebserverAddressLength + maxFilenameLength;

class D2BlockUpdater : public QObject
{
	Q_OBJECT

private:

	QString url;
	QString httpServer;
	QString updateFile;
	QString ignorelistFile;
	int currentRevision;

	bool ignoreListOutOfDate;

	QFile* file;

	void ProcessRegistryInformation();
	void ProcessVersionFile();
	bool IgnoreListIsOutOfDate();
	void UpdateIgnoreListFile();

signals:
	void UpdaterComplete();

public slots:
	void UpdateIgnoreList();

public:

	D2BlockUpdater();
	~D2BlockUpdater();
};
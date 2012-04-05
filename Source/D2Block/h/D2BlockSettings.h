#pragma once

#include <QSettings>

class D2BlockSettings : public QSettings
{
	Q_OBJECT

public:

	D2BlockSettings();
	~D2BlockSettings();

	const QString Server() const;
	const QString RevisionFile() const;
	const QString IgnorelistFile() const;
	const QString LaunchTarget() const;
	const qint32 LocalRevision() const;

	void setServer(const QString& server);
	void setRevisionFile(const QString& file);
	void setIgnorelistFile(const QString& file);
	void setLaunchTarget(const QString& target);
	void setLocalRevision(const qint32& revision);

private:
	
	QSettings* m_applicationSettings;

	const QString m_iniFileName;

	const QString m_serverString;
	const QString m_revisionFileString;
	const QString m_ignorelistFileString;
	const QString m_launchTargetString;
	const QString m_localRevisionString;

};


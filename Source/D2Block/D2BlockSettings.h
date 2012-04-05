#pragma once

#include <QSettings>

class D2BlockSettings : public QSettings
{
	Q_OBJECT

public:
	D2BlockSettings(QObject *parent = nullptr);
	~D2BlockSettings();

	const QString Server() const;
	const QString RevisionFile() const;
	const QString IgnorelistFile() const;
	const QString LaunchTarget() const;
	const quint32 LocalRevision() const;

	void setServer(const QString& server) const;
	void setRevisionFile(const QString& file) const;
	void setIgnorelistFile(const QString& file) const;
	void setLaunchTarget(const QString& target) const;
	void setLocalRevision(const qint32& revision) const;

private:
	
	QSettings* m_applicationSettings;
	const QString m_iniFileName;

};


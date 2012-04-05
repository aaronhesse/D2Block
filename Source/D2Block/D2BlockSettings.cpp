#include "StdAfx.h"
#include "D2BlockSettings.h"

D2BlockSettings::D2BlockSettings(QObject *parent):
QSettings(parent),
m_iniFileName(QCoreApplication::applicationDirPath() + "/d2block.ini")
{
	m_applicationSettings = new QSettings(m_iniFileName, QSettings::IniFormat);
}

D2BlockSettings::~D2BlockSettings()
{
	delete m_applicationSettings;
	m_applicationSettings = nullptr;
}

const QString D2BlockSettings::Server() const
{
	return m_applicationSettings->value("Server").toString();
}

const QString D2BlockSettings::RevisionFile() const
{
	return m_applicationSettings->value("RevisionFile").toString();
}

const QString D2BlockSettings::IgnorelistFile() const
{
	return m_applicationSettings->value("IgnorelistFile").toString();
}

const QString D2BlockSettings::LaunchTarget() const
{
	return m_applicationSettings->value("LaunchTarget").toString();
}

const quint32 D2BlockSettings::LocalRevision() const
{
	return m_applicationSettings->value("LocalRevision").toInt();
}

void D2BlockSettings::setServer(const QString& server) const
{
	m_applicationSettings->setValue("LocalRevision", server);
}

void D2BlockSettings::setRevisionFile(const QString& file) const
{
	m_applicationSettings->setValue("LocalRevision", file);
}

void D2BlockSettings::setIgnorelistFile(const QString& file) const
{
	m_applicationSettings->setValue("LocalRevision", file);
}

void D2BlockSettings::setLaunchTarget(const QString& target) const
{
	m_applicationSettings->setValue("LocalRevision", target);
}

void D2BlockSettings::setLocalRevision(const qint32& revision) const
{
	m_applicationSettings->setValue("LocalRevision", revision);
}
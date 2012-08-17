#include "StdAfx.h"
#include "D2BlockSettings.h"

D2BlockSettings::D2BlockSettings():
    QSettings(),
    m_iniFileName(QCoreApplication::applicationDirPath() + "/d2block.ini"),
    m_serverString("Server"),
    m_revisionFileString("RevisionFile"),
    m_ignorelistFileString("IgnorelistFile"),
    m_launchTargetString("LaunchTarget"),
    m_localRevisionString("LocalRevision"),
    m_gameInstallPathString("GameInstallPath"),
    m_applicationSettings(m_iniFileName, QSettings::IniFormat)
{
#if defined(Q_OS_WIN)

    // For Windows we can read out the installPath from the registry upon construction.
    // For Mac we have to have the user specify the path, so we handle that in the
    // appropriate function.

    QSettings gameSettings("Blizzard Entertainment", "Diablo II");
    installPath  = gameSettings.value("InstallPath").toString();
    processPath  = gameSettings.value("GamePath").toString();
    setLaunchTarget(processPath);

#endif
}

D2BlockSettings::~D2BlockSettings()
{
}

D2BlockSettings* D2BlockSettings::getInstance()
{
    static D2BlockSettings* instance = 0;

    if (!instance)
        instance = new D2BlockSettings();

    return instance;
}

const QString D2BlockSettings::iniFileName() const
{
    return m_iniFileName;
}

const QString D2BlockSettings::Server() const
{
    return m_applicationSettings.value(m_serverString).toString();
}

const QString D2BlockSettings::RevisionFile() const
{
    return m_applicationSettings.value(m_revisionFileString).toString();
}

const QString D2BlockSettings::IgnorelistFile() const
{
    return m_applicationSettings.value(m_ignorelistFileString).toString();
}

const QString D2BlockSettings::LaunchTarget() const
{
    return m_applicationSettings.value(m_launchTargetString).toString();
}

const qint32 D2BlockSettings::LocalRevision() const
{
    return m_applicationSettings.value(m_localRevisionString).toInt();
}

const QString D2BlockSettings::GameInstallPath() const
{
    return m_applicationSettings.value(m_gameInstallPathString).toString();
}

void D2BlockSettings::setServer(const QString& server)
{
    m_applicationSettings.setValue(m_serverString, server);
}

void D2BlockSettings::setRevisionFile(const QString& file)
{
    m_applicationSettings.setValue(m_revisionFileString, file);
}

void D2BlockSettings::setIgnorelistFile(const QString& file)
{
    m_applicationSettings.setValue(m_ignorelistFileString, file);
}

void D2BlockSettings::setLaunchTarget(const QString& target)
{
    m_applicationSettings.setValue(m_launchTargetString, target);
}

void D2BlockSettings::setLocalRevision(const qint32& revision)
{
    m_applicationSettings.setValue(m_localRevisionString, revision);
}

void D2BlockSettings::setGameInstallPath(const QString& path)
{
    m_applicationSettings.setValue(m_gameInstallPathString, path);
}

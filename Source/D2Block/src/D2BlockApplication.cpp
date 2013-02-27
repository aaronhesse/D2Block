#include "StdAfx.h"
#include "D2BlockApplication.h"
#include "D2BlockSettings.h"

D2BlockApplication::D2BlockApplication(int argc, char *argv[]):
    QApplication(argc, argv)
{
    ProcessCommandlineArguments();
    ConfigureSettings();
}

D2BlockApplication::~D2BlockApplication()
{
}

void D2BlockApplication::ProcessCommandlineArguments()
{
    // Iterate over the pass-through command-line arguments, store them in a class member QStringList
    // Skip the first argument though because that's the fully qualified path of this process.

    const QStringList arguments = QCoreApplication::arguments();
    foreach (QString arg, arguments)
    {
        if (arg.compare(arguments.at(0)) == 0)
            continue;

        m_passThroughCommandlineArguments.push_back(arg);
    }
}

void D2BlockApplication::ConfigureSettings()
{
    D2BlockSettings* settings = D2BlockSettings::getInstance();

    if (!QFile::exists(settings->iniFileName()))
    {
        const QString gamePath = QSettings("Blizzard Entertainment", "Diablo II").value("GamePath").toString();

        settings->setServer("scourcritical.s3.amazonaws.com/D2Block");
        settings->setRevisionFile("revision.txt");
        settings->setIgnorelistFile("ignorelist");
        settings->setLocalRevision(0);
        settings->setLaunchTarget(gamePath);
    }
}

void D2BlockApplication::on_updateComplete()
{
	emit setProgressBar(100);
    LaunchLaunchTarget();
    exit();
}

void D2BlockApplication::LaunchLaunchTarget()
{
    emit setProgressTitle(tr("Launching Launch Target..."));

#if defined(Q_OS_WIN)
    LaunchWindowsLaunchTarget();
#elif defined(Q_OS_MAC)
    LaunchMacLaunchTarget();
#endif

}

void D2BlockApplication::LaunchWindowsLaunchTarget()
{
    // If no launch target key/value is found, then we use Diablo II.exe/app as the launch target.
    // Otherwise use the user-defined target.

    const QString launchTargetPath = D2BlockSettings::getInstance()->LaunchTarget();
    const QString processPath  = launchTargetPath;
    QString filePathText = launchTargetPath;
    QString installPath  = QDir::toNativeSeparators(launchTargetPath);
    installPath.truncate(installPath.lastIndexOf(QDir::separator()));

    foreach (QString argument, m_passThroughCommandlineArguments)
        filePathText.append(" " + argument);

    emit setFilePathText(filePathText);

	QDir::setCurrent(installPath);

    QProcess gameProcess;
    gameProcess.startDetached(processPath, m_passThroughCommandlineArguments);
}

void D2BlockApplication::LaunchMacLaunchTarget()
{
    // If no launch target key/value is found, then we use Diablo II.exe/app as the launch target.
    // Otherwise use the user-defined target.

    const QString launchTargetPath = D2BlockSettings::getInstance()->LaunchTarget();

    QString processPath  = launchTargetPath;
    QString filePathText = launchTargetPath;
    QString installPath  = QDir::toNativeSeparators(launchTargetPath);
    installPath.truncate(installPath.lastIndexOf(QDir::separator()));

    foreach (QString argument, m_passThroughCommandlineArguments)
    {
        filePathText.append(" " + argument);
        processPath.append(" " + argument);
    }

    emit setFilePathText(filePathText);

    system(QString("open \"" + processPath + "\"").toAscii());
}

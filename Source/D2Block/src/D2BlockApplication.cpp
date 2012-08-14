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

    QStringList arguments = QCoreApplication::arguments();
    foreach (QString arg, arguments)
    {
        if (arg.compare(arguments.at(0)) == 0)
            continue;

        m_passThroughCommandlineArguments.push_back(arg);
    }
}

void D2BlockApplication::ConfigureSettings() const
{
    D2BlockSettings settings;

    if (!QFile::exists(settings.iniFileName()))
    {
        settings.setServer("cloud.github.com/downloads/aaronhesse/d2block");
        settings.setRevisionFile("revision.txt");
        settings.setIgnorelistFile("ignorelist");
        settings.setLocalRevision(0);
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
    // If no launch target key/value is found, then we use Diablo II.exe/app as the launch target.
    // Otherwise use the user-defined target.

    QString processPath;
    QString installPath;
    QString filePathText;

    emit setProgressTitle(tr("Launching Launch Target..."));

    const QString launchTargetPath = D2BlockSettings().LaunchTarget();

    if (!launchTargetPath.isEmpty())
    {
        processPath = launchTargetPath;
        installPath = QDir::toNativeSeparators(launchTargetPath);
        installPath.truncate(installPath.lastIndexOf(QDir::separator()));
        filePathText = launchTargetPath;
    }
    else
    {
#if defined(Q_OS_WIN)
        QSettings gameSettings("Blizzard Entertainment", "Diablo II");
        installPath  = gameSettings.value("InstallPath").toString();
        processPath  = gameSettings.value("GamePath").toString();

		if (processPath.isEmpty())
			processPath = installPath.append("Diablo II.exe");

        filePathText = processPath;
#elif defined(Q_OS_MAC)
        // QSettings objects don't properly support reading .prefs files, which Diablo II uses.
        // So we have to manually set the launchTarget here.
        installPath  = D2BlockSettings().GameInstallPath();
        processPath  = installPath;
        processPath  = processPath.append("Diablo II (Carbon)");
        filePathText = processPath;
#endif
        // Once we've defined what the processPath is we write it out to the .ini file for next time.
        D2BlockSettings().setLaunchTarget(processPath);
    }

    foreach (QString argument, m_passThroughCommandlineArguments)
        filePathText.append(" " + argument);

    emit setFilePathText(filePathText);

    QProcess gameProcess;
    gameProcess.setWorkingDirectory(installPath);
    gameProcess.startDetached(processPath, m_passThroughCommandlineArguments);
}

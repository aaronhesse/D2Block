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
	foreach(QString arg, arguments)
	{
		if(arg.compare(arguments.at(0)) == 0)
			continue;

		m_passThroughCommandlineArguments.push_back(arg);
	}
}

void D2BlockApplication::ConfigureSettings() const
{
	D2BlockSettings settings;

	if (!QFile::exists(settings.iniFileName()))
	{
		const QString gamePath = QSettings("Blizzard Entertainment", "Diablo II").value("GamePath").toString();

		settings.setServer("cloud.github.com/downloads/aaronhesse/d2block");
		settings.setRevisionFile("revision.txt");
		settings.setIgnorelistFile("ignorelist");
		settings.setLocalRevision(0);
		settings.setLaunchTarget(gamePath);
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
	// If no launch target key/value is found, then we use Diablo II.exe as a the launch target.
	// Otherwise use the user-defined target.

	QString processPath;
	QString installPath;
	QString filePathText;

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
		QSettings gameSettings("Blizzard Entertainment", "Diablo II");
		installPath = gameSettings.value("InstallPath").toString();
		processPath = gameSettings.value("GamePath").toString();
		filePathText = processPath;
	}

	foreach(QString argument, m_passThroughCommandlineArguments)
		filePathText.append(" " + argument);

	emit setProgressTitle("Launching Launch Target...");
	emit setFilePathText(filePathText);
	QCoreApplication::processEvents();

	QProcess gameProcess;
	gameProcess.setWorkingDirectory(installPath);
	gameProcess.startDetached(processPath, m_passThroughCommandlineArguments);
}

void D2BlockApplication::Sleep(const quint32& ms)
{
#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

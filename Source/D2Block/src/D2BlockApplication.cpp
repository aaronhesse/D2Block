#include "StdAfx.h"
#include "D2BlockApplication.h"
#include "D2BlockSettings.h"

D2BlockApplication::D2BlockApplication(int argc, char *argv[]):
QApplication(argc, argv)
{
	ProcessCommandlineArguments(argc, argv);
	ConfigureSettings();
}

D2BlockApplication::~D2BlockApplication(void)
{
}

void D2BlockApplication::ProcessCommandlineArguments(int argc, char *argv[])
{
	// Iterate over the pass-through command-line arguments, store them in a class member QStringList
	// Skip the first argument though because that's the fully qualified path of this process.
	for(int i = 1; i < argc; i++)
	{
		m_passThroughCommandlineArguments.push_back(argv[i]);
	}
}

void D2BlockApplication::ConfigureSettings() const
{
	D2BlockSettings settings;
	
	bool iniFileExists = true;
	if(settings.Server().isEmpty())
		iniFileExists = false;

	if (!iniFileExists)
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

	QString launchTargetPath = D2BlockSettings().LaunchTarget();

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
	gameProcess.start(processPath, m_passThroughCommandlineArguments);
	gameProcess.waitForFinished(60000);
}

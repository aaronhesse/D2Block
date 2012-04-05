#include "StdAfx.h"
#include "D2BlockApplication.h"

const QString D2BlockApplication::m_diablo2Executable = "Diablo II.exe";

D2BlockApplication::D2BlockApplication(int argc, char *argv[]):
QApplication(argc, argv)
{
	ProcessCommandlineArguments(argc, argv);
	SetupRegistryEntries();
}

D2BlockApplication::~D2BlockApplication(void)
{
}

void D2BlockApplication::ProcessCommandlineArguments(int argc, char *argv[])
{
	argumentCount = argc;
	argumentValues = argv;

	// Iterate over the pass-through command-line arguments, store them in a class member QStringList
	// Skip the first argument though because that's the fully qualified path of this process.
	for(int i = 1; i < argumentCount; i++)
	{
		m_passThroughCommandlineArguments.push_back(argumentValues[i]);
	}
}

void D2BlockApplication::SetupRegistryEntries()
{
	QSettings settings(QSettings::SystemScope, "D2Block", "D2Block");
	
	bool firstRun = false;
	if(settings.value("Server").toString().isEmpty())
		firstRun = true;

	if (firstRun)
	{
		settings.setValue("Server", "cloud.github.com/downloads/aaronhesse/d2block");
		settings.setValue("Revision File", "revision.txt");
		settings.setValue("Ignorelist File", "ignorelist");
		settings.setValue("Local Revision", 0);
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

	QString launchTargetPath = QSettings("D2Block","D2Block").value("Launch Target").toString();

	if (!launchTargetPath.isEmpty())
	{
		processPath = launchTargetPath;
		installPath = QDir::toNativeSeparators(launchTargetPath);
		installPath.truncate(installPath.lastIndexOf(QDir::separator()));
		filePathText = launchTargetPath;
	}
	else
	{
		QSettings gameSettings("Blizzard Entertainment","Diablo II");
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
	gameProcess.waitForFinished(10000);
}

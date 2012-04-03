#include "StdAfx.h"
#include "D2BlockApplication.h"

const QString D2BlockApplication::m_diablo2Executable = "Diablo II.exe";

D2BlockApplication::D2BlockApplication(int argc, char *argv[]):
QApplication(argc, argv)
{
	ProcessCommandlineArguments(argc, argv);
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

void D2BlockApplication::on_updateComplete()
{
	emit setProgressBar(100);
	LaunchDiablo2();
	exit();
}

void D2BlockApplication::LaunchDiablo2() const
{
	QProcess gameProcess;
	QString executablePath = QCoreApplication::applicationDirPath();
	executablePath.sprintf("%s\\%s", executablePath, m_diablo2Executable);
	gameProcess.start(m_diablo2Executable, m_passThroughCommandlineArguments);
}

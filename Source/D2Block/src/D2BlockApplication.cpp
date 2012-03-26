#include "StdAfx.h"
#include "D2BlockApplication.h"
#include "D2BlockUpdater.h"

const QString D2BlockApplication::diablo2Executable = "Diablo II.exe";

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

	// Iterate over the pass-through command-line arguments, store them in a class member
	// Skip the first argument though because that's the fully qualified path of this process.
	// Make sure to chop off the final excess space at the end.
	for(int i = 1; i < argumentCount; i++)
	{
		passThroughCommandlineArguments.append(argumentValues[i]);
		passThroughCommandlineArguments.append(" ");
	}
	passThroughCommandlineArguments.chop(1);
}

void D2BlockApplication::on_updateComplete()
{
	// set OK button text to (3) ... (2) ... (1)
	LaunchDiablo2();
	exit();
}

int D2BlockApplication::LaunchDiablo2() const
{
	QString executablePath;
	GetCurrentDirectory(MAX_PATH, (LPSTR)executablePath.data()); // use QCoreApplication getApplicationDirectory?
	executablePath.sprintf("%s\\%s", executablePath, diablo2Executable);

	//return (int)ShellExecute(NULL, "open", executablePath.toAscii(), passThroughCommandlineArguments.toAscii(), NULL, SW_SHOW);
	// use QDesktopServices
	return 0;
}
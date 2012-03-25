#include "StdAfx.h"
#include "D2BlockWindow.h"
#include "D2BlockApplication.h"
#include "D2BlockUpdater.h"

int main(int argc, char *argv[])
{
	D2BlockApplication app(argc, argv);
	D2BlockWindow win;

	win.ShowWindow();

	QThread workerThread;
	D2BlockUpdater updater(argc, argv);
	updater.moveToThread(&workerThread);
	QObject::connect(&workerThread, SIGNAL(started()), &updater, SLOT(UpdateIgnoreList()), Qt::QueuedConnection);
	QObject::connect(&updater, SIGNAL(UpdaterComplete()), &app, SLOT(HandleUpdaterComplete()), Qt::QueuedConnection);
	workerThread.start();
	
	return app.exec();
}

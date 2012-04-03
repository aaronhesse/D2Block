#include "StdAfx.h"
#include "D2BlockWindow.h"
#include "D2BlockApplication.h"
#include "D2BlockUpdater.h"

static void primaryWorkerThreadFunc(D2BlockUpdater* app);

int main(int argc, char *argv[])
{
	D2BlockApplication app(argc, argv);
	D2BlockUpdater updater;
	D2BlockWindow win;

	QObject::connect(&app, SIGNAL(updateOkButtonText()), &win, SLOT(on_updateOkButtonText()));
	QObject::connect(&win, SIGNAL(countdownComplete()), &app, SLOT(on_countdownComplete()));
	QObject::connect(&updater, SIGNAL(UpdaterComplete()), &app, SLOT(on_updateComplete()));
	QObject::connect(&updater, SIGNAL(updateProgressBar(qint32)), &win, SLOT(on_updateProgressBar(qint32)));
	QObject::connect(&app, SIGNAL(setProgressBar(qint32)), &win, SLOT(on_setProgressBar(qint32)));

	win.ShowWindow();

	QtConcurrent::run(primaryWorkerThreadFunc, &updater);
	
	return app.exec();
}

static void primaryWorkerThreadFunc(D2BlockUpdater* updater)
{
	updater->UpdateIgnoreList();
}

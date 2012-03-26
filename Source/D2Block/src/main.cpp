#include "StdAfx.h"
#include "D2BlockWindow.h"
#include "D2BlockApplication.h"
#include "D2BlockUpdater.h"

static void primaryWorkerThreadFunc(D2BlockApplication* app);

int main(int argc, char *argv[])
{
	D2BlockApplication app(argc, argv);
	D2BlockWindow win;

	win.ShowWindow();

	QtConcurrent::run(primaryWorkerThreadFunc, &app);
	
	return app.exec();
}

static void primaryWorkerThreadFunc(D2BlockApplication* app)
{
	D2BlockUpdater updater;

	QObject::connect(&updater, SIGNAL(UpdaterComplete()), app, SLOT(on_updateComplete()));

	updater.UpdateIgnoreList();
}

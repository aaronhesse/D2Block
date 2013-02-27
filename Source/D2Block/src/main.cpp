#include "StdAfx.h"
#include "D2BlockWindow.h"
#include "D2BlockUpdater.h"
#include "D2BlockApplication.h"
#include "D2BlockLogitechLCD.h"
#include <QtConcurrentRun>

static void updaterWorkerThreadFunc(D2BlockUpdater*);
static void lcdHandlerWorkerThreadFunc(D2BlockLogitechLCD*);

int main(int argc, char *argv[])
{
    D2BlockApplication app(argc, argv);
	D2BlockLogitechLCD lcdHandler;
    D2BlockUpdater updater;
    D2BlockWindow window;

    QObject::connect(&app,     SIGNAL(setProgressBar(qint32)),            &window,        SLOT(on_setProgressBar(qint32)));
    QObject::connect(&app,     SIGNAL(setProgressTitle(QString)),         &window,        SLOT(on_setProgressTitle(QString)));
    QObject::connect(&app,     SIGNAL(setFilePathText(QString)),          &window,        SLOT(on_setFilePathText(QString)));
	QObject::connect(&app,     SIGNAL(setProgressBar(qint32)),            &lcdHandler,    SLOT(on_setProgressBar(qint32)));
	QObject::connect(&app,     SIGNAL(setProgressTitle(QString)),         &lcdHandler,    SLOT(on_setProgressTitle(QString)));

    QObject::connect(&updater, SIGNAL(updaterComplete()),                 &app,           SLOT(on_updateComplete()));
    QObject::connect(&updater, SIGNAL(showProgressBar()),                 &window,        SLOT(on_showProgressBar()));
    QObject::connect(&updater, SIGNAL(updateProgressBar(qint32)),         &window,        SLOT(on_updateProgressBar(qint32)));
    QObject::connect(&updater, SIGNAL(setProgressTitle(QString)),         &window,        SLOT(on_setProgressTitle(QString)));
    QObject::connect(&updater, SIGNAL(setFilePathText(QString)),          &window,        SLOT(on_setFilePathText(QString)));
    QObject::connect(&updater, SIGNAL(diablo2NotInstalled()),             &window,        SLOT(on_diablo2NotInstalled()));
	QObject::connect(&updater, SIGNAL(updateProgressBar(qint32)),         &lcdHandler,    SLOT(on_updateProgressBar(qint32)));
	QObject::connect(&updater, SIGNAL(setProgressTitle(QString)),         &lcdHandler,    SLOT(on_setProgressTitle(QString)));

    QObject::connect(&window,  SIGNAL(setGameInstallPath(const QString)), &updater,       SLOT(on_setGameInstallPath(const QString)));

    window.show();

	QtConcurrent::run(lcdHandlerWorkerThreadFunc, &lcdHandler);
    QtConcurrent::run(updaterWorkerThreadFunc, &updater);

    return app.exec();
}

static void updaterWorkerThreadFunc(D2BlockUpdater* updater)
{
	updater->UpdateIgnoreList();
}

static void lcdHandlerWorkerThreadFunc(D2BlockLogitechLCD* lcdHandler)
{
	lcdHandler->HandleLogitechLCD();
}
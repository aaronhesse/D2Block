#include "StdAfx.h"
#include "D2BlockWindow.h"
#include "D2BlockUpdater.h"
#include "D2BlockApplication.h"
#include <QtConcurrentRun>

static void primaryWorkerThreadFunc(D2BlockUpdater* app);

int main(int argc, char *argv[])
{
    D2BlockApplication app(argc, argv);
    D2BlockUpdater updater;
    D2BlockWindow window;

    QObject::connect(&app,     SIGNAL(setProgressBar(qint32)),            &window,  SLOT(on_setProgressBar(qint32)));
    QObject::connect(&app,     SIGNAL(setProgressTitle(QString)),         &window,  SLOT(on_setProgressTitle(QString)));
    QObject::connect(&app,     SIGNAL(setFilePathText(QString)),          &window,  SLOT(on_setFilePathText(QString)));

    QObject::connect(&updater, SIGNAL(updaterComplete()),                 &app,     SLOT(on_updateComplete()));
    QObject::connect(&updater, SIGNAL(showProgressBar()),                 &window,  SLOT(on_showProgressBar()));
    QObject::connect(&updater, SIGNAL(updateProgressBar(qint32)),         &window,  SLOT(on_updateProgressBar(qint32)));
    QObject::connect(&updater, SIGNAL(setProgressTitle(QString)),         &window,  SLOT(on_setProgressTitle(QString)));
    QObject::connect(&updater, SIGNAL(setFilePathText(QString)),          &window,  SLOT(on_setFilePathText(QString)));
    QObject::connect(&updater, SIGNAL(diablo2NotInstalled()),             &window,  SLOT(on_diablo2NotInstalled()));

    QObject::connect(&window,  SIGNAL(setGameInstallPath(const QString)), &updater, SLOT(on_setGameInstallPath(const QString)));

    window.show();

    QtConcurrent::run(primaryWorkerThreadFunc, &updater);

    return app.exec();
}

static void primaryWorkerThreadFunc(D2BlockUpdater* updater)
{
    updater->UpdateIgnoreList();
}

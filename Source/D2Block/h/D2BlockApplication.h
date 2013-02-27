#pragma once

#include <QtGui/QApplication>
#include <QStringList>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class D2BlockApplication : public QApplication
{
    Q_OBJECT

public:

    D2BlockApplication(int argc, char *argv[]);
    ~D2BlockApplication();

public slots:

    void on_updateComplete();

private:

    QStringList m_passThroughCommandlineArguments;

    void ProcessCommandlineArguments();
    void ConfigureSettings();
    void LaunchLaunchTarget();
    void LaunchMacLaunchTarget();
    void LaunchWindowsLaunchTarget();

signals:

    void setProgressBar(const qint32&);
    void setFilePathText(const QString&);
    void setProgressTitle(const QString&);
};

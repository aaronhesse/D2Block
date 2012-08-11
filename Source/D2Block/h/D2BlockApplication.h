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

    static void Sleep(const quint32& ms);

public slots:

    void on_updateComplete();

private:

    QStringList m_passThroughCommandlineArguments;

    void ProcessCommandlineArguments();
    void ConfigureSettings() const;
    void LaunchLaunchTarget();

signals:

    void setProgressBar(const qint32& value);
    void setFilePathText(const QString& text);
    void setProgressTitle(const QString& text);
};

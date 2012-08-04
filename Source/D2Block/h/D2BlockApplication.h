#pragma once

#include <QtGui/QApplication>
#include <QStringList>

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
	void ConfigureSettings() const;
	void LaunchLaunchTarget();

signals:

	void setProgressBar(const qint32& value);
	void setFilePathText(const QString& text);
	void setProgressTitle(const QString& text);
};

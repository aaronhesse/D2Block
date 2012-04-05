#pragma once

#include <QtGui/QApplication>

class D2BlockApplication : public QApplication
{
	Q_OBJECT

private:

	QStringList m_passThroughCommandlineArguments;

	void ProcessCommandlineArguments(int argc, char* argv[]);
	void ConfigureSettings() const;
	void LaunchLaunchTarget();

signals:
	void setProgressBar(qint32 value);
	void setFilePathText(QString text);
	void setProgressTitle(QString text);

public slots:
	void on_updateComplete();

public:
	D2BlockApplication(int argc, char *argv[]);
	~D2BlockApplication(void);

	void RunUpdater() const;
};


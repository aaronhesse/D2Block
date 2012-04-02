#pragma once

#include <QtGui/QApplication>

class D2BlockApplication : public QApplication
{
	Q_OBJECT

private:
	int argumentCount;
	char** argumentValues;

	static const size_t maxGameArgumentLength = 64;
	static const QString diablo2Executable;

	QString passThroughCommandlineArguments;

	void ProcessCommandlineArguments(int argc, char *argv[]);
	bool LaunchDiablo2() const;

	public slots:
		void on_updateComplete();

public:
	D2BlockApplication(int argc, char *argv[]);
	~D2BlockApplication(void);

	void RunUpdater() const;
};


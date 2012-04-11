#pragma once

#include <QtGui/QMainWindow>
#include "ui_d2block.h"

class D2BlockWindow : public QMainWindow
{
	Q_OBJECT

public:

	D2BlockWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~D2BlockWindow();

	void show();

public slots:

	void on_setProgressBar(const qint32& value);
	void on_updateProgressBar(const qint32& amount);
	void on_setProgressTitle(const QString& text);
	void on_setFilePathText(const QString& text);

private:

	Ui::D2BlockClass ui;
};

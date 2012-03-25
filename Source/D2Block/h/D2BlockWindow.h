#ifndef D2BLOCKWINDOW_H
#define D2BLOCKWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_d2block.h"

class D2BlockWindow : public QMainWindow
{
	Q_OBJECT

public:
	D2BlockWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~D2BlockWindow();

	void ShowWindow();

private:
	Ui::D2BlockClass ui;
};

#endif // D2BLOCKWINDOW_H

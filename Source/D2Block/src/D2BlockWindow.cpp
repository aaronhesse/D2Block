#include "stdafx.h"
#include "d2blockWindow.h"

D2BlockWindow::D2BlockWindow(QWidget *parent, Qt::WFlags flags):
QMainWindow(parent, flags)
{
	ui.setupUi(this);
	ui.progressBar->setValue(0);
}

D2BlockWindow::~D2BlockWindow()
{

}

void D2BlockWindow::ShowWindow()
{
	setFixedSize(width(), height());
	show();
}

void D2BlockWindow::on_updateProgressBar(qint32 amount)
{
	qint32 currentValue = ui.progressBar->value();
	ui.progressBar->setValue(currentValue + amount);
}

void D2BlockWindow::on_setProgressBar(qint32 value)
{
	ui.progressBar->setValue(value);
}
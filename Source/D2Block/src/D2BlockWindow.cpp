#include "stdafx.h"
#include "d2blockWindow.h"

D2BlockWindow::D2BlockWindow(QWidget *parent, Qt::WFlags flags):
QMainWindow(parent, flags)
{
	ui.setupUi(this);
}

D2BlockWindow::~D2BlockWindow()
{

}

void D2BlockWindow::ShowWindow()
{
	setFixedSize(width(), height());
	show();
}
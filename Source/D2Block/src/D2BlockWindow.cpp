#include "StdAfx.h"
#include "D2BlockWindow.h"
#include "D2BlockApplication.h"

D2BlockWindow::D2BlockWindow(QWidget *parent, Qt::WFlags flags):
	QMainWindow(parent, flags)
{
	setWindowFlags(Qt::WindowCloseButtonHint);
	ui.setupUi(this);
	ui.progressBar->setValue(0);
	ui.filePath->setText("");
}

D2BlockWindow::~D2BlockWindow()
{

}

void D2BlockWindow::show()
{
	setFixedSize(width(), height());
	QMainWindow::show();
}

void D2BlockWindow::on_updateProgressBar(const qint32& amount)
{
	qint32 currentValue = ui.progressBar->value();
	ui.progressBar->setValue(currentValue + amount);
}

void D2BlockWindow::on_setProgressBar(const qint32& value)
{
	ui.progressBar->setValue(value);
}

void D2BlockWindow::on_setFilePathText(const QString& path)
{
	ui.filePath->setText(path);
}

void D2BlockWindow::on_setProgressTitle(const QString& text)
{
	ui.progressTitle->setText(text);
}

void D2BlockWindow::on_diablo2NotInstalled()
{
	ui.progressBar->hide();
	ui.filePath->hide();
	ui.progressTitle->setText("Diablo II Is Not Installed");

	qApp->processEvents();

    D2BlockApplication::Sleep(static_cast<quint32>(3000));
}

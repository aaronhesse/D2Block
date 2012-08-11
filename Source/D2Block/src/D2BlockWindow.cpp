#include "StdAfx.h"
#include "D2BlockWindow.h"

D2BlockWindow::D2BlockWindow(QWidget *parent, Qt::WFlags flags):
    QMainWindow(parent, flags),
    m_menuBar(new QMenuBar(0)),
    m_menu(new QMenu()),
    m_setGameInstallPathAction(new QAction("Set Diablo II InstallPath", this))
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui.setupUi(this);
    ui.progressBar->setValue(0);
    ui.filePath->setText("");

    m_setGameInstallPathAction->setMenuRole(QAction::ApplicationSpecificRole);
    m_menu->addAction(m_setGameInstallPathAction);
    m_menuBar->addMenu(m_menu);

    QObject::connect(m_setGameInstallPathAction, SIGNAL(triggered()), this, SLOT(on_chooseGameInstallPath()));
}

D2BlockWindow::~D2BlockWindow()
{
}

void D2BlockWindow::show()
{
    setFixedSize(width(), height());
    QMainWindow::show();
}

void D2BlockWindow::on_chooseGameInstallPath()
{
    QString directory;
    QFileDialog directoryDialog(this);
    directoryDialog.setFileMode(QFileDialog::Directory);
    directoryDialog.setOption(QFileDialog::ShowDirsOnly, true);

    if (directoryDialog.exec())
        directory = directoryDialog.selectedFiles()[0];

    emit setGameInstallPath(directory);
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
    ui.filePath->setText("Please install Diablo II or set the Installation Path.");
    ui.progressTitle->setText("Diablo II Is Not Installed");

    qApp->processEvents();
}

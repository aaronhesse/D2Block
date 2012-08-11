#pragma once

#include <QtGui/QMainWindow>
#include <QMenuBar>
#include <QFileDialog>
#include "ui_d2block.h"

class D2BlockWindow : public QMainWindow
{
    Q_OBJECT

public:

    D2BlockWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~D2BlockWindow();

    void show();

signals:

    void setGameInstallPath(const QString&);

public slots:

    void on_setProgressBar(const qint32& value);
    void on_updateProgressBar(const qint32& amount);
    void on_setProgressTitle(const QString&);
    void on_setFilePathText(const QString&);
    void on_diablo2NotInstalled();
    void on_chooseGameInstallPath();

private:

    Ui::D2BlockClass ui;

    QMenuBar* m_menuBar;
    QMenu*    m_menu;
    QAction*  m_setGameInstallPathAction;
};

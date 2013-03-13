#pragma once

#include "lglcd.h"
#include "EZ_LCD.h"
#include "LCDBase.h"
#include "LCDText.h"
#include "LCDProgressBar.h"

class D2BlockLogitechLCD : public QObject
{
	Q_OBJECT

public:

	D2BlockLogitechLCD();
	~D2BlockLogitechLCD();

	void HandleLogitechLCD();

private:

	CLCDPage m_mainPage;
	CLCDText m_titleText;
	CLCDText m_progressText;
	CLCDProgressBar m_progressBar;

	CLCDConnection m_lcdConnection;
	lgLcdSoftbuttonsChangedContext m_sbContext;

	void ConnectToLogitechLCD();
	void OpenLogitechLCD();
	void AddUIElements();
	void HandleMessageLoop();

public slots:

	void on_updateProgressBar(const qint32&);
	void on_setProgressBar(const qint32&);
	void on_setProgressTitle(const QString&);

};


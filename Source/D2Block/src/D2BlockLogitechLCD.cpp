#include "StdAfx.h"
#include "D2BlockLogitechLCD.h"

#include <assert.h>

D2BlockLogitechLCD::D2BlockLogitechLCD()
{
}

D2BlockLogitechLCD::~D2BlockLogitechLCD()
{
	m_lcdConnection.Shutdown();
}

void D2BlockLogitechLCD::HandleLogitechLCD()
{
	ConnectToLogitechLCD();
	AddUIElements();
	HandleMessageLoop();
}

void D2BlockLogitechLCD::ConnectToLogitechLCD()
{
	lgLcdConnectContextEx connectCtx;

	memset(&connectCtx, 0, sizeof(connectCtx));
	connectCtx.appFriendlyName = "D2Block";
	connectCtx.isAutostartable = FALSE;
	connectCtx.isPersistent = FALSE;

	connectCtx.onConfigure.configCallback = nullptr;
	connectCtx.onConfigure.configContext = nullptr;

	connectCtx.dwAppletCapabilitiesSupported = LGLCD_APPLET_CAP_BW;

	m_lcdConnection.Initialize(connectCtx, &m_sbContext);
}

void D2BlockLogitechLCD::AddUIElements()
{
	auto* pMonoOutput = m_lcdConnection.MonoOutput();

	pMonoOutput->ShowPage(&m_mainPage);

	m_titleText.SetText("D2Block");
	m_titleText.SetOrigin(50,0);
	m_titleText.SetSize(160,20);
	m_titleText.SetFontPointSize(14);

	m_progressText.SetOrigin(15,20);
	m_progressText.SetSize(160,20);
	m_progressText.SetFontPointSize(8);

	m_progressBar.SetOrigin(0,35);
	m_progressBar.SetSize(160,8);
	m_progressBar.SetProgressStyle(CLCDProgressBar::STYLE_FILLED);
	m_progressBar.SetRange(0,100);

	m_mainPage.AddObject(&m_titleText);
	m_mainPage.AddObject(&m_progressText);
	m_mainPage.AddObject(&m_progressBar);
}

void D2BlockLogitechLCD::HandleMessageLoop()
{
	MSG msg;
	BOOL done = FALSE;

	while (!done)									
	{
		// Here we check the value of the progressBar to see if it's full. This fixes
		// an issue where the message loop isn't being exited properly even when PostQuitMessage
		// is called elsewhere, which causes a number of crashes to occur later inside the
		// m_lcdConnection.Update() call.

		if (m_progressBar.GetPos() == 100)
		{
			done = TRUE;
			continue;
		}

		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )	
		{
			if (msg.message == WM_QUIT)
			{
				done = TRUE;
			}
			else									
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// The Update() will do the rendering of any LCDUI objects we added to pages
			// Also this loop goes very fast, so let's throttle it a bit.

			m_lcdConnection.Update();
			Sleep(33);
		}
	}
}

void D2BlockLogitechLCD::on_setProgressTitle(const QString& text)
{
	m_progressText.SetText(text.toStdString().c_str());
}

void D2BlockLogitechLCD::on_setProgressBar(const qint32& amount)
{
	m_progressBar.SetPos(amount);
}

void D2BlockLogitechLCD::on_updateProgressBar(const qint32& amount)
{
	qint32 currentValue = m_progressBar.GetPos();
	m_progressBar.SetPos(currentValue + amount);
}
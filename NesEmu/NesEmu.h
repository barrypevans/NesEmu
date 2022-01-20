#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NesEmu.h"
#include "MemoryWidget.h"
#include "DisassemblyWidget.h"
#include "ScreenWidget.h"
#include "PatternTableWidget.h"
#include "Nes.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/qpushbutton.h>

#include <QThread>

class NesEmu : public QMainWindow
{
    Q_OBJECT

public:
    NesEmu(QWidget *parent = Q_NULLPTR);
	
	void UpdateEmulation();

	void LoadCartridge();
	void Tick();
	void Reset();

	void UIUpdate();

private:

	void SetupButton(QPushButton* pButton);
	void UnpauseEmulation();
	void PauseEmulation();
	void ClearRam();

    Ui::NesEmuClass ui;
	MemoryWidget* m_pMemoryWidget;
	DisassemblyWidget* m_pDisassemblyWidget;
	Nes* m_pNes;

	QHBoxLayout* m_layout;
	QPushButton* m_pClockButton;
	QPushButton* m_pPauseButton;
	QPushButton* m_pStartButton;
	QPushButton* m_pResetButton;

	ScreenWidget* m_pScreenWidget;
	PatternTableWidget* m_pPatternTableWidget;

	QIcon m_clockIcon;
	QIcon m_resetIcon;
	QIcon m_startIcon;
	QIcon m_pauseIcon;

	bool m_running;

	const static int kButtonWidth;
	const static int kButtonHeight;
	const static int kIconSize;
	const static QString kTitle;
};

class EmulationThread : public QThread
{
	Q_OBJECT
public:
	EmulationThread(NesEmu* pEmu);
	NesEmu* pEmulator;
private:
	void run();
};
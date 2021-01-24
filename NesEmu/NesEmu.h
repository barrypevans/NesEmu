#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NesEmu.h"
#include "MemoryWidget.h"
#include "DisassemblyWidget.h"
#include "Nes.h"

#include <QHBoxLayout>
#include <qpushbutton.h>

class NesEmu : public QMainWindow
{
    Q_OBJECT

public:
    NesEmu(QWidget *parent = Q_NULLPTR);
	
	void UpdateEmulation();

	/*void UploadProgram();*/
	void LoadCartridge();
	void Tick();
	void Reset();
private:

	void SetupButton(QPushButton* pButton);
	void UnpauseEmulation();
	void PauseEmulation();

    Ui::NesEmuClass ui;
	MemoryWidget* m_pMemoryWidget;
	DisassemblyWidget* m_pDisassemblyWidget;
	Nes* m_pNes;

	QHBoxLayout* m_layout;
	QPushButton* m_pClockButton;
	QPushButton* m_pPauseButton;
	QPushButton* m_pStartButton;
	QPushButton* m_pResetButton;

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

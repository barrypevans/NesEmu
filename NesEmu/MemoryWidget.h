#pragma once
#include <qwidget.h>
#include <qlabel.h>
#include <QTableWidget>
#include "cpu6502.h"
#include "Bus.h"

class MemoryWidget : public QWidget
{
public:
	MemoryWidget();
	~MemoryWidget();

	void UpdateState(Cpu6502* pCpu, Bus* pBus);

private:

	void SetupRamTable();
	void SetupRegTable();

	void SetTableSize(QTableWidget* pTable);

	QTableWidgetItem* GetNewTableItem(QString text = "00");

	QLabel* m_pRamView;
	QLabel* m_pRegisterView;
	QString m_ramString;
	QTableWidget* m_pRegisterTable;
	QTableWidget* m_pRamTable;
};


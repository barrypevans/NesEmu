#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/QTableWidget>
#include "cpu6502.h"
#include "Bus.h"
#include "Inbox.h"

class MemoryWidget : public QWidget , public Inbox
{
public:
	MemoryWidget();
	~MemoryWidget();

	void UpdateState(Cpu6502* pCpu, Bus* pBus, bool fullUpdate = false);
	void UpdateDatum(uint16_t addr, uint8_t datum);

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

	// Inherited via Inbox
	virtual void RecieveMessage(std::string label, void * payload) override;
};


#pragma once
#include <qwidget.h>
#include <qlabel.h>
#include <QListWidget>
#include "Utils.h"

class DisassemblyWidget : public QWidget
{
public:
	DisassemblyWidget();
	~DisassemblyWidget();

	void SetDissasembly(Bus* pCpuBus, uint16_t startAddr, uint16_t length);
	bool Update(uint16_t pc);

private:
	QListWidgetItem* GetNewListItem(QString text);

	std::vector<Utils::DisassembledInstruction> m_disassembly;
	QListWidget* m_pDisassemblyList;
};


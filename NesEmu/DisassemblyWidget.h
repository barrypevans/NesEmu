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

	void SetDissasembly(uint8_t* bin, uint16_t size);
	bool Update(uint16_t pc);

private:
	QListWidgetItem* GetNewListItem(QString text);

	std::vector<Utils::DisassembledInstruction> m_disassembly;
	QListWidget* m_pDisassemblyList;
};


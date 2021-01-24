#include "DisassemblyWidget.h"

#include <QVBoxLayout>
#include <vector>
#include <QFrame>

DisassemblyWidget::DisassemblyWidget()
{
	m_pDisassemblyList = new QListWidget();
	m_pDisassemblyList->setMaximumWidth(200);
	m_pDisassemblyList->setMinimumWidth(200);

	// setup title label
	QLabel* pTitle = new QLabel("Disassembly\n");
	auto titleFont = pTitle->font();
	titleFont.setPointSize(8);
	titleFont.setBold(true);
	pTitle->setFont(titleFont);
	pTitle->setMinimumHeight(20);

	QFrame* pFrame = new QFrame();
	pFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
	pFrame->setLayout(new QVBoxLayout());

	
	setLayout(new QVBoxLayout());
	pFrame->layout()->addWidget(pTitle);
	pFrame->layout()->addWidget(m_pDisassemblyList);
	layout()->addWidget(pFrame);
}

DisassemblyWidget::~DisassemblyWidget()
{
}

void DisassemblyWidget::SetDissasembly(Bus* pCpuBus, uint16_t startAddr, uint16_t length)
{
	m_pDisassemblyList->clear();
	m_disassembly = Utils::Dissassemble(pCpuBus, startAddr, length);
	for (int i = 0; i < m_disassembly.size(); ++i)
	{
		Utils::DisassembledInstruction inst = m_disassembly[i];
		auto pItem = GetNewListItem(inst.mnemonic);
		pItem->setFlags(pItem->flags() | Qt::ItemIsUserCheckable);
		pItem->setCheckState(Qt::Unchecked);
		m_pDisassemblyList->addItem(pItem);
	}
}

QListWidgetItem * DisassemblyWidget::GetNewListItem(QString text)
{
	QListWidgetItem* item = new QListWidgetItem();
	item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
	item->setText(text);
	return item;
}

bool DisassemblyWidget::Update(uint16_t pc)
{
	// deselect all
	for (int i = 0; i < m_pDisassemblyList->count(); ++i)
		m_pDisassemblyList->item(i)->setFlags(m_pDisassemblyList->item(i)->flags() & ~Qt::ItemIsSelectable);

	for (int i = 0; i < m_pDisassemblyList->count(); ++i)
	{
		QListWidgetItem* item = m_pDisassemblyList->item(i);
		auto pInst = m_disassembly[i];
	
		if (pc >= pInst.start && pc <= pInst.end)
		{
			item->setFlags(item->flags() | Qt::ItemIsSelectable);
			item->setSelected(true);
			return item->checkState();
		}
	}
	return false;
}

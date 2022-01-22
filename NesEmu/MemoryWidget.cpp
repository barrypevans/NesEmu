#include "MemoryWidget.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include "Utils.h"

MemoryWidget::MemoryWidget()
{
	QFrame* pFrame = new QFrame();
	pFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
	pFrame->setLayout(new QVBoxLayout());

	SetupRegTable();
	SetupRamTable();

	QLabel* pRegTitle = new QLabel("Registers");
	auto titleFont = pRegTitle->font();
	titleFont.setPointSize(8);
	titleFont.setBold(true);
	pRegTitle->setFont(titleFont);
	pRegTitle->setMinimumHeight(20);

	QLabel* pRamTitle = new QLabel("Ram");
	pRamTitle->setFont(titleFont);
	pRamTitle->setMinimumHeight(20);

	setLayout(new QVBoxLayout());
	pFrame->layout()->addWidget(pRegTitle);
	pFrame->layout()->addWidget(m_pRegisterTable);
	pFrame->layout()->addWidget(pRamTitle);
	pFrame->layout()->addWidget(m_pRamTable);
	layout()->addWidget(pFrame);

	Init();
}

MemoryWidget::~MemoryWidget()
{
}

void MemoryWidget::UpdateState(Cpu6502 * pCpu, Bus * pBus, bool fullUpdate)
{
	QTableWidgetItem *item = m_pRegisterTable->item(0, 0);
	item->setText(Utils::IntToHexString(pCpu->A));
	item = m_pRegisterTable->item(0, 1);
	item->setText(Utils::IntToHexString(pCpu->X));
	item = m_pRegisterTable->item(0, 2);
	item->setText(Utils::IntToHexString(pCpu->Y));
	item = m_pRegisterTable->item(0, 3);
	item->setText(Utils::IntToHexString(pCpu->status));
	item = m_pRegisterTable->item(0, 4);
	item->setText(Utils::IntToHexString(pCpu->sp, 4));
	item = m_pRegisterTable->item(0, 5);
	item->setText(Utils::IntToHexString(pCpu->pc, 4));

	int flagIndex = 0;
	while (flagIndex < 8)
	{
		item = m_pRegisterTable->item(0, 6 + flagIndex);
		item->setText((pCpu->status & (1 << flagIndex)) > 0 ? "1" : "0");
		flagIndex++;
	}


	/*if (fullUpdate)
	{
		int row = 0;
		while (row < 2048)
		{
			int col = 0;
			while (col < 32)
			{
				int index = row * 32 + col;
				int value = pBus->Read(index);

				QString stringVal = Utils::IntToHexString(value);
				item = m_pRamTable->item(row, col);
				item->setText(stringVal);
				++col;
			}
			++row;
		}
	}*/
}

void MemoryWidget::UpdateDatum(uint16_t addr, uint8_t datum)
{
	/*int row = addr / 32;
	int col = addr % 32;
	QString stringVal = Utils::IntToHexString(datum);
	QTableWidgetItem* item = m_pRamTable->item(row, col);
	item->setText(stringVal);*/
}

void MemoryWidget::SetupRamTable()
{
	m_pRamTable = new QTableWidget(2048, 32);
	int col = 0;
	int row = 0;
	while (col < 32)
	{
		m_pRamTable->setColumnWidth(col, 1);
		auto header = m_pRamTable->horizontalHeader();
		header->setSectionResizeMode(col, QHeaderView::Fixed);

		// Set hor header values
		QTableWidgetItem *item = new QTableWidgetItem();
		item->setText(Utils::IntToHexString(col));
		m_pRamTable->setHorizontalHeaderItem(col, item);
		col++;
	}
	while (row < 2048)
	{
		m_pRamTable->setRowHeight(row, 1);
		auto header = m_pRamTable->verticalHeader();
		header->setSectionResizeMode(row, QHeaderView::Fixed);

		// Set vert header values
		QTableWidgetItem *item = new QTableWidgetItem();
		item->setText(Utils::IntToHexString(row * 32, 4));
		m_pRamTable->setVerticalHeaderItem(row, item);
		row++;
	}

	// set not editable
	row = 0;
	while (row < 2048)
	{
		col = 0;
		while (col < 32)
		{
			QTableWidgetItem *item = new QTableWidgetItem();
			m_pRamTable->setItem(row, col, item);
			item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
			item->setText("00");
			item->setTextAlignment(Qt::AlignCenter);
			++col;
		}
		++row;
	}
	SetTableSize(m_pRamTable);
}

void MemoryWidget::SetupRegTable()
{
	m_pRegisterTable = new QTableWidget(1, 14);
	m_pRegisterTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	const QString kRegNames[14] = { "A", "X", "Y", "ST", "SP", "PC", "C" , "Z" , "I" , "D", "B" , "U" , "V" , "N" };

	int headerIndex = 0;
	while (headerIndex < 14)
	{
		m_pRegisterTable->setHorizontalHeaderItem(headerIndex, GetNewTableItem(kRegNames[headerIndex]));
		m_pRegisterTable->setItem(0, headerIndex, GetNewTableItem(headerIndex < 6 ? "00" : "0"));
		m_pRegisterTable->setColumnWidth(headerIndex, 1);
		headerIndex++;
	}

	m_pRegisterTable->setVerticalHeaderItem(1, GetNewTableItem(""));
	SetTableSize(m_pRegisterTable);
}

void MemoryWidget::SetTableSize(QTableWidget * pTable)
{
	QRect rect = pTable->geometry();
	int tableWidth = 2 + pTable->verticalHeader()->width();
	for (int i = 0; i < pTable->columnCount(); i++) {
		tableWidth += pTable->columnWidth(i);
	}
	int tableHeight = 2 + pTable->horizontalHeader()->height();
	for (int i = 0; i < pTable->rowCount(); i++) {
		tableHeight += pTable->rowHeight(i);
	}
	pTable->setMaximumHeight(tableHeight);
	pTable->setMaximumWidth(tableWidth);
}

QTableWidgetItem * MemoryWidget::GetNewTableItem(QString text)
{
	QTableWidgetItem *pItem = new QTableWidgetItem();
	pItem->setFlags(pItem->flags() &  ~Qt::ItemIsEditable);
	pItem->setText(text);
	return pItem;
}

void MemoryWidget::RecieveMessage(std::string label, void * payload)
{
	if (label == "BusDatumUpdated")
	{
		Bus::BusDatumPayload* busPayload = (Bus::BusDatumPayload*)payload;
		UpdateDatum(busPayload->addr, busPayload->datum);
	}
}

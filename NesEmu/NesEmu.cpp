#include "NesEmu.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTableWidget>
#include "Utils.h"

const QString NesEmu::kTitle = "NesEmu - %1";
const int NesEmu::kButtonWidth = 35;
const int NesEmu::kButtonHeight = 30;
const int NesEmu::kIconSize = 25;

NesEmu::NesEmu(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	QHBoxLayout* pMainLayout = new QHBoxLayout();
	QVBoxLayout* pToolBarLayout = new QVBoxLayout();
	QHBoxLayout* pButtonLayout = new QHBoxLayout();
	m_layout = new QHBoxLayout();
	m_pScreenWidget = new ScreenWidget();
	m_pPatternTableWidget = new PatternTableWidget();
	QWidget* centralWidget = new QWidget();
	centralWidget->setLayout(m_layout);
	setCentralWidget(centralWidget);

	m_clockIcon = QIcon("./icons/footsteps-silhouette-variant.png");
	m_startIcon = QIcon("./icons/play-button-arrowhead.png");
	m_pauseIcon = QIcon("./icons/pause.png");
	m_resetIcon = QIcon("./icons/reset.png");
	m_pClockButton = new QPushButton(m_clockIcon, "");
	m_pResetButton = new QPushButton(m_resetIcon, "");
	m_pPauseButton = new QPushButton(m_pauseIcon, "");
	m_pStartButton = new QPushButton(m_startIcon, "");

	SetupButton(m_pClockButton);
	SetupButton(m_pResetButton);
	SetupButton(m_pPauseButton);
	SetupButton(m_pStartButton);

	m_pNes = new Nes();
	m_pMemoryWidget = new MemoryWidget();
	m_pDisassemblyWidget = new DisassemblyWidget();

	m_layout->addLayout(pToolBarLayout);
	pToolBarLayout->addLayout(pButtonLayout);
	pToolBarLayout->addLayout(pMainLayout);
	pButtonLayout->addStretch();
	pButtonLayout->addWidget(m_pResetButton);
	pButtonLayout->addWidget(m_pPauseButton);
	pButtonLayout->addWidget(m_pStartButton);
	pButtonLayout->addWidget(m_pClockButton);
	pButtonLayout->addStretch();
	pMainLayout->addWidget(m_pMemoryWidget, Qt::AlignHCenter);
	pMainLayout->addWidget(m_pDisassemblyWidget);

	m_pScreenWidget->show();
	m_pPatternTableWidget->show();

	m_running = false;

	connect(m_pClockButton, &QPushButton::released, this, [=]() 
	{
		do
		{
			Tick(); Tick(); Tick();
		} while (m_pNes->m_pCpu->remainingCycles > 0);
		m_pDisassemblyWidget->SetDissasembly(m_pNes->m_pCpuBus, m_pNes->m_pCpu->pc, 10); });
	connect(m_pResetButton, &QPushButton::released, this, &NesEmu::Reset);
	connect(m_pPauseButton, &QPushButton::released, this, &NesEmu::PauseEmulation);
	connect(m_pStartButton, &QPushButton::released, this, &NesEmu::UnpauseEmulation);
	connect(ui.actionLoad_Program, &QAction::triggered, this, &NesEmu::LoadCartridge);
	connect(ui.actionClear_Ram, &QAction::triggered, this, &NesEmu::ClearRam);
}

void NesEmu::UpdateEmulation()
{
	if(m_running)
		Tick();
}

void NesEmu::LoadCartridge()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"), "C:/", tr("INes Rom (*.nes)"));
	if (fileNames.size() > 0)
	{
		QString filename = fileNames[0];
		std::string stdFileName = filename.toStdString();
		m_pNes->InsertCartridge(stdFileName);

		if (m_pNes->HasCartridge())
		{
			m_pNes->Reset();

			m_pDisassemblyWidget->SetDissasembly(m_pNes->m_pCpuBus, m_pNes->m_pCpu->pc, 20);

			m_pMemoryWidget->UpdateState(m_pNes->m_pCpu, m_pNes->m_pCpuBus, true);

			setWindowTitle(kTitle.arg(filename));
		}
	}
}

void NesEmu::Tick()
{

	if (m_pScreenWidget->m_controllerValues != m_pNes->m_pIOBusInterface->m_controllerState[0])
	{
		qDebug() << m_pNes->m_pPpu->m_pOamRaw[0];
	}
	m_pNes->m_pIOBusInterface->m_controllerState[0] = m_pScreenWidget->m_controllerValues;
	

	m_pNes->Tick();
	m_pScreenWidget->Render(m_pNes->m_pPpu);
	m_pPatternTableWidget->Render(m_pNes->m_pPpu);

	// Breakpoint hit
	/*if (m_pDisassemblyWidget->Update(m_pNes->m_pCpu->pc))
	{
		PauseEmulation();
	}*/
}

void NesEmu::Reset()
{
	m_pNes->Reset();
	m_pMemoryWidget->UpdateState(m_pNes->m_pCpu, m_pNes->m_pCpuBus, true);
	m_pDisassemblyWidget->Update(m_pNes->m_pCpu->pc);
}

void NesEmu::UIUpdate()
{
	m_pScreenWidget->UIRender();
	m_pPatternTableWidget->UIRender();
	m_pMemoryWidget->UpdateState(m_pNes->m_pCpu, m_pNes->m_pCpuBus);
	m_pDisassemblyWidget->SetDissasembly(m_pNes->m_pCpuBus, m_pNes->m_pCpu->pc, 10);
	m_pDisassemblyWidget->Update(m_pNes->m_pCpu->pc);
}

void NesEmu::SetupButton(QPushButton * pButton)
{
	pButton->setIconSize(QSize(kIconSize, kIconSize));
	pButton->setMinimumHeight(kButtonHeight);
	pButton->setMinimumWidth(kButtonWidth);
	pButton->setMaximumHeight(kButtonHeight);
	pButton->setMaximumWidth(kButtonWidth);
}

void NesEmu::UnpauseEmulation()
{
	m_running = true;
}

void NesEmu::PauseEmulation()
{
	m_running = false;
	m_pMemoryWidget->UpdateState(m_pNes->m_pCpu, m_pNes->m_pCpuBus, true);
}

void NesEmu::ClearRam()
{
	m_pNes->m_pCpuRam->Clear();
	m_pMemoryWidget->UpdateState(m_pNes->m_pCpu, m_pNes->m_pCpuBus, true);
}

EmulationThread::EmulationThread(NesEmu* pEmu)
{
	pEmulator = pEmu;
}

void EmulationThread::run()
{
	while (pEmulator->isVisible())
	{
		pEmulator->UpdateEmulation();
	}
}
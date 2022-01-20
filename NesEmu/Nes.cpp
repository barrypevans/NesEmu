#include "Nes.h"

Nes::Nes()
{
	m_pCpuBus = new Bus();
	m_pCpuRam = new Ram(0x0800);

	m_pPpuBus = new Bus();
	m_pNameTableRam = new Ram(0x1000);
	m_pPaletteRam = new Ram(0x0100);

	// hook up on board ppu ram
	m_pPpuBus->RegisterMemoryDevice(m_pNameTableRam, 0x2000);
	m_pPpuBus->RegisterMemoryDevice(m_pNameTableRam, 0x3000, 0x3EFF); // 2k name table is mirrored
	m_pPpuBus->RegisterMemoryDevice(m_pPaletteRam, 0x3F00);

	// first 2k of ram mirror across the first 8k of address bus
	m_pCpuBus->RegisterMemoryDeviceMirror(m_pCpuRam, 0x0000, 3);


	// hook cpu and ppu up to buses
	m_pCpu = new Cpu6502(m_pCpuBus, m_pPpuBus);
	m_pPpu = new Ppu2C02(m_pPpuBus, m_pCpuBus);

	m_pCpuBus->RegisterMemoryDeviceMirror(m_pPpu->m_pRegisterInterface, 0x2000, 400);


	m_tickCount = 0;
}

Nes::~Nes()
{
	delete m_pCpu;
	delete m_pCpuRam;
	delete m_pCpuBus;
}

void Nes::Tick()
{
	m_pPpu->Tick();
	if ((m_tickCount % 3) == 0)
		m_pCpu->Tick();
	m_tickCount++;
}

void Nes::Reset()
{
	m_pCpu->Reset();
}

void Nes::InsertCartridge(std::string romPath)
{
	if (m_pCart) RemoveCartridge();

	m_pCart = new Cartridge(romPath, m_pCpuBus, m_pPpuBus);

	// If cartrigfe fails to load, remove it.
	if (!m_pCart->IsCatridgeReady()) RemoveCartridge();
}

void Nes::RemoveCartridge()
{
	if (m_pCart) delete m_pCart;
	m_pCart = nullptr;
}

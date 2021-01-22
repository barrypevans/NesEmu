#include "Nes.h"

Nes::Nes()
{
	m_pCpuBus		= new Bus();
	m_pCpuRam		= new Ram(0x0200);
	m_pProgamMemory = new Ram(0x0800);
	
	m_pPpuBus		= new Bus();
	m_pNameTableRam = new Ram(0x1000);
	m_pPaletteRam	= new Ram(0x0100);

	// hook up on board ppu ram
	m_pPpuBus->RegisterMemoryDevice(m_pNameTableRam, 0x2000);
	m_pPpuBus->RegisterMemoryDevice(m_pPaletteRam, 0x3F00);

	// first 2k of ram mirror across the first 8k of address bus
	m_pCpuBus->RegisterMemoryDevice(m_pCpuRam, 0x0000);
	m_pCpuBus->RegisterMemoryDevice(m_pCpuRam, 0x0800);
	m_pCpuBus->RegisterMemoryDevice(m_pCpuRam, 0x1000);
	m_pCpuBus->RegisterMemoryDevice(m_pCpuRam, 0x1800);

	// program memory
	m_pCpuBus->RegisterMemoryDevice(m_pProgamMemory, 0x4020);

	// 2 byte inital pc location
	m_pCpuBus->RegisterMemoryDevice(new Ram(0x0002), 0xFFFC);
	m_pCpuBus->Write(0xFFFC, 0x20);
	m_pCpuBus->Write(0xFFFD, 0x40);

	m_pCpu = new Cpu6502(m_pCpuBus, m_pPpuBus);
	m_pPpu = new Ppu2C02(m_pPpuBus, m_pCpuBus);
}

Nes::~Nes()
{
	delete m_pCpu;
	delete m_pCpuRam;
	delete m_pCpuBus;
}

void Nes::Tick()
{
	m_pCpu->Tick();
}

void Nes::Reset()
{
	m_pCpu->Reset();
}

void Nes::UploadProgram(uint8_t * bin, uint16_t size)
{
	for (int p = 0; p < size; ++p)
	{
		m_pCpuBus->Write(0x4020 + p, bin[p]);
	}
	m_programSize = size;
}

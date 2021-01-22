#include "Nes.h"

Nes::Nes()
{
	m_pBus = new Bus();
	m_pRam = new Ram(0x0800);
	m_pProgamMemory = new Ram(0x0800);
	
	// first 2k of ram
	m_pBus->RegisterMemoryDevice(m_pRam, 0x0000);
	// temp program memory
	m_pBus->RegisterMemoryDevice(m_pProgamMemory, 0x0800);
	// 2 byte inital pc location
	m_pBus->RegisterMemoryDevice(new Ram(0x0002), 0xFFFC);

	m_pCpu = new Cpu6502(m_pBus);
}

Nes::~Nes()
{
	delete m_pCpu;
	delete m_pRam;
	delete m_pBus;
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
		m_pBus->Write(0x0800 + p, bin[p]);
	}
	m_pBus->Write(0xFFFC, 0x00);
	m_pBus->Write(0xFFFD, 0x08);
	m_programSize = size;
}

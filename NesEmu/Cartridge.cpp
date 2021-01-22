#include "Cartridge.h"

Cartridge::Cartridge(std::string romPath, Bus * m_pCpuBus, Bus * m_pPpuBus)
{
	m_cartridgeReady = false;

	//TODO: Read iNes header
	//TODO: Initialize mapper

	m_pCpuBus->RegisterMemoryDevice(m_pCpuInterface, 0x4020);
	m_pPpuBus->RegisterMemoryDevice(m_pPpuInterface, 0x0000);
	m_cartridgeReady = true;
}

Cartridge::~Cartridge()
{
	if (m_pMapper)
		delete(m_pMapper);
}

uint8_t Cartridge::CpuRead(uint16_t addr)
{
	uint16_t finalAddr;
	if(m_pMapper->CpuRemap(addr, finalAddr))
		return m_pPrgMemory[finalAddr];

	return 0x00;
}

bool Cartridge::CpuWrite(uint16_t addr, uint8_t data)
{
	uint16_t finalAddr;
	if (m_pMapper->CpuRemap(addr, finalAddr))
	{
		m_pPrgMemory[finalAddr] = data;
		return true;
	}
	return false;
}

uint8_t Cartridge::PpuRead(uint16_t addr)
{
	uint16_t finalAddr;
	if (m_pMapper->PpuRemap(addr, finalAddr))
		return m_pChrMemory[finalAddr];

	return 0x00;
}

bool Cartridge::PpuWrite(uint16_t addr, uint8_t data)
{
	uint16_t finalAddr;
	if (m_pMapper->PpuRemap(addr, finalAddr))
	{
		m_pChrMemory[finalAddr] = data;
		return true;
	}
	return false;
}

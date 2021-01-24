#include "Cartridge.h"
#include "Mapper000.h"

Cartridge::Cartridge(std::string romPath, Bus * pCpuBus, Bus * pPpuBus):m_pCpuBus(pCpuBus), m_pPpuBus(pPpuBus)
{
	m_cartridgeReady = false;

	// Load Rom
	INes::ReadINesFile(romPath, m_pPrgMemory, m_pChrMemory, m_iNesHeader);
	if (!m_pPrgMemory || !m_pChrMemory) return;

	if (!InitMapper()) return;
	
	// connect mapper to bus
	m_pCpuInterface = new CartridgeCPUInterface(this);
	m_pPpuInterface = new CartridgePPUInterface(this);
	m_pCpuBus->RegisterMemoryDevice(m_pCpuInterface, 0x4020);
	m_pPpuBus->RegisterMemoryDevice(m_pPpuInterface, 0x0000);

	m_cartridgeReady = true;
}

Cartridge::~Cartridge()
{
	m_pCpuBus->UnregisterMemoryDevice(m_pCpuInterface);
	m_pPpuBus->UnregisterMemoryDevice(m_pPpuInterface);

	if (m_pCpuInterface) delete m_pCpuInterface;
	if (m_pPpuInterface) delete m_pPpuInterface;
	if (m_pMapper)		 delete m_pMapper;
	if (m_pPrgMemory)	 free(m_pPrgMemory);
	if (m_pChrMemory)	 free(m_pChrMemory);
}

uint8_t Cartridge::CpuRead(uint16_t addr)
{
	uint16_t finalAddr;
	if (m_pMapper->CpuRemap(addr, finalAddr))
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

bool Cartridge::InitMapper()
{
	m_mapperId = ((m_iNesHeader.mapper2 >> 4) << 4) | (m_iNesHeader.mapper1 << 4);
	switch (m_mapperId)
	{
	case 0:
		m_pMapper = new Mapper000(m_iNesHeader);
		return true;
	}

	return false;
}

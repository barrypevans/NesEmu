#include "Cartridge.h"

Cartridge::Cartridge(std::string romPath, Bus * m_pCpuBus, Bus * m_pPpuBus)
{
	m_cartridgeReady = false;
	//TODO: Read iNes header
	//TODO: Initialize mapper

	m_pCpuBus->RegisterMemoryDevice(m_pMapper->m_pCpuInterface, 0x4020);
	m_pPpuBus->RegisterMemoryDevice(m_pMapper->m_pPpuInterface, 0x0000);

}

Cartridge::~Cartridge()
{
	if (m_pMapper)
		delete(m_pMapper);
}

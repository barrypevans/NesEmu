#include "IOBusInterface.h"

uint8_t IOBusInterface::Read(uint16_t addr)
{
	if (addr == 0x16 || addr == 0x17)
	{
		uint8_t controllerIndex = addr & 0x01;
		uint8_t data = (m_internalControllerState[controllerIndex] & 0x80)>0;
		m_internalControllerState[controllerIndex] <<= 1;
		return data;
	}

	return m_pData[addr];
}

bool IOBusInterface::Write(uint16_t addr, uint8_t data)
{
	if (addr == 0x14) 
	{
		m_dmaShouldStart = true;
		m_dmaPage = data;
	}
	else if (addr == 0x16 || addr == 0x17)
	{
		uint8_t controllerIndex = addr & 0x01;
		m_internalControllerState[controllerIndex] = m_controllerState[controllerIndex];
	}


	return true;
}

uint16_t IOBusInterface::GetSize()
{
	return 0x18;
}

bool IOBusInterface::UseVirtualAddressSpace()
{
	return true;
}

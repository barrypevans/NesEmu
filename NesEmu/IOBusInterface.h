#pragma once
#include "Bus.h"


class IOBusInterface : public IMemoryDevice
{
	// Inherited via IMemoryDevice
	virtual uint8_t Read(uint16_t addr) override;
	virtual bool Write(uint16_t addr, uint8_t data) override;
	virtual uint16_t GetSize() override;
	virtual bool UseVirtualAddressSpace() override;
	uint8_t m_pData[0x18];
public:
	bool m_dmaShouldStart;
	uint8_t m_dmaPage;
	uint8_t m_controllerState[2];

private:
	uint8_t m_internalControllerState[2];
};

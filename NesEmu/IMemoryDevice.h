#pragma once
#include <stdint.h>

class IMemoryDevice
{
public:
	virtual uint8_t Read(uint16_t addr) = 0;
	virtual bool Write(uint16_t addr, uint8_t data) = 0;
	virtual uint16_t GetSize() = 0;
	virtual bool UseVirtualAddressSpace() = 0;
};


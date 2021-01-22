#pragma once
#include "IMemoryDevice.h"

class Bus
{
public:
	Bus();
	~Bus();

	uint8_t Read(uint16_t addr);
	uint16_t Read16(uint16_t addr);
	bool Write(uint16_t addr, uint8_t data);
	
	void RegisterMemoryDevice(IMemoryDevice* device, uint16_t virtualBusOffset);

	const static uint8_t kMaxMemoryDevices = 16;
	const static uint16_t kBusSize = 0xFFFF;
	
private:

	struct MemoryDeviceMapping
	{
		IMemoryDevice*	m_pDevice;
		uint16_t		m_virtualBusOffset;
	};
	MemoryDeviceMapping m_memoryDeviceMappings[kMaxMemoryDevices];
	MemoryDeviceMapping kNullDevice = { nullptr, 0 };
	
	MemoryDeviceMapping& FetchMemoryInterfaceAtAddress(uint16_t addr);
	bool DeviceExistsInRange(uint16_t startAddr, uint16_t endAddr);

	uint8_t m_numMemoryDevices = 0;
};


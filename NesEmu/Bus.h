#pragma once
#include "IMemoryDevice.h"
#include "Inbox.h"
#include <vector>
class Bus
{
public:
	Bus();
	~Bus();

	struct BusDatumPayload
	{
		uint16_t addr;
		uint8_t datum;
	};

	uint8_t Read(uint16_t addr);
	uint16_t Read16(uint16_t addr, bool bug = false);
	bool Write(uint16_t addr, uint8_t data);
	
	void RegisterMemoryDevice(IMemoryDevice* device, uint16_t virtualBusOffset);
	void RegisterMemoryDeviceMirror(IMemoryDevice* device, uint16_t virtualBusOffset, uint8_t mirrorCount = 0);
	void RegisterMemoryDevice(IMemoryDevice* device, uint16_t virtualBusOffset, uint16_t endAddress, uint8_t mirrorCount = 0);
	void UnregisterMemoryDevice(IMemoryDevice* device);

	const static uint8_t kMaxMemoryDevices = 16;
	const static uint32_t kBusSize = 0x10000;
	
private:

	struct MemoryDeviceMapping
	{
		IMemoryDevice*	m_pDevice;
		uint16_t		m_virtualBusOffset;
		uint16_t		m_endAddress;
		uint8_t			m_mirrorCount = 0;
	};
	MemoryDeviceMapping m_memoryDeviceMappings[kMaxMemoryDevices];
	MemoryDeviceMapping kNullDevice = { nullptr, 0 };
	
	MemoryDeviceMapping& FetchMemoryInterfaceAtAddress(uint16_t addr);
	bool DeviceExistsInRange(uint16_t startAddr, uint16_t endAddr);
	uint16_t TranslateAddress(MemoryDeviceMapping& mapping, uint16_t address);
	uint8_t m_numMemoryDevices = 0;
	std::vector<Inbox> m_memChangedCallback;
};


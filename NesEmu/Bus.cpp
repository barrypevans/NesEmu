#include "Bus.h"
#include <assert.h>
#include "Messager.h"

Bus::Bus()
{
}

Bus::~Bus()
{
}

uint8_t Bus::Read(uint16_t addr)
{
	// Fetch memory device
	MemoryDeviceMapping& memoryDeviceMapping = FetchMemoryInterfaceAtAddress(addr);

	// Trying to read from a section of memory with nothing mapped :(
	// This should probably just return garbage and warn rather than crashing.
	//assert(memoryDeviceMapping.m_pDevice != nullptr);
	if (memoryDeviceMapping.m_pDevice == nullptr)
	{
		// garbage
		return 0x00;
	}
	// Subtract the virtual offset from the proposed address to read from the correct position in the 
	// memory device's virtual address space
	uint16_t finalAddr = memoryDeviceMapping.m_pDevice->UseVirtualAddressSpace() ? addr - memoryDeviceMapping.m_virtualBusOffset : addr;
	return memoryDeviceMapping.m_pDevice->Read(finalAddr);
}

uint16_t Bus::Read16(uint16_t addr, bool bug)
{
	if (!bug)
	{
		uint16_t low = Read(addr);
		uint16_t high = Read(addr + 1);
		return  (high << 8) | low;
	}
	else
	{
		// emulates a 6502 bug that caused the low byte to wrap without
		// incrementing the high byte
		return  (Read(addr & 0xFF00) << 8) | Read(addr);
	}

}

bool Bus::Write(uint16_t addr, uint8_t data)
{
	// Fetch memory device
	MemoryDeviceMapping& memoryDeviceMapping = FetchMemoryInterfaceAtAddress(addr);
	if (memoryDeviceMapping.m_pDevice == nullptr)
		return false;

	// Subtract the virtual offset from the proposed address to read from the correct position in the 
	// memory device's virtual address space
	uint16_t finalAddr = memoryDeviceMapping.m_pDevice->UseVirtualAddressSpace() ? addr - memoryDeviceMapping.m_virtualBusOffset : addr;
	
	if (memoryDeviceMapping.m_pDevice->Write(finalAddr, data))
	{
		// update ui with this datum
		BusDatumPayload payload = { finalAddr, data };
		Messager::Get()->SendMessage("BusDatumUpdated", &payload);
	}
		
	return true;
}

void Bus::RegisterMemoryDevice(IMemoryDevice * pDevice, uint16_t virtualBusOffset)
{
	// Make sure this device does not map outside the bus
	assert(pDevice->GetSize() + virtualBusOffset <= kBusSize);

	// Make sure we dont add overlapping devices
	assert(!DeviceExistsInRange(virtualBusOffset, virtualBusOffset + pDevice->GetSize()-1));

	// Add new device to the device list
	m_memoryDeviceMappings[m_numMemoryDevices] = { pDevice, virtualBusOffset };
	m_numMemoryDevices++;
}

void Bus::UnregisterMemoryDevice(IMemoryDevice * pDevice)
{
	uint8_t deviceIndex = 0;
	while (deviceIndex < m_numMemoryDevices)
	{
		MemoryDeviceMapping& deviceMapping = m_memoryDeviceMappings[deviceIndex];
		if (deviceMapping.m_pDevice == pDevice)
		{
			m_numMemoryDevices--;
			if (deviceIndex != m_numMemoryDevices)
				m_memoryDeviceMappings[deviceIndex] = m_memoryDeviceMappings[m_numMemoryDevices];
			else
				break;
		}
		else
		{
			deviceIndex++;
		}
	}
}

Bus::MemoryDeviceMapping& Bus::FetchMemoryInterfaceAtAddress(uint16_t addr)
{
	uint8_t deviceIndex = 0;
	while (deviceIndex < m_numMemoryDevices)
	{
		MemoryDeviceMapping& deviceMapping = m_memoryDeviceMappings[deviceIndex];
		uint16_t virtualEndAddress = deviceMapping.m_virtualBusOffset + deviceMapping.m_pDevice->GetSize();
		// Checking if the given address falls between the start and end of this devices address space
		if (deviceMapping.m_virtualBusOffset <= addr && virtualEndAddress > addr)
			return deviceMapping;
		deviceIndex++;
	}
	return kNullDevice;
}

bool Bus::DeviceExistsInRange(uint16_t startAddr, uint16_t endAddr)
{
	uint8_t deviceIndex = 0;
	while (deviceIndex < m_numMemoryDevices)
	{
		MemoryDeviceMapping& deviceMapping = m_memoryDeviceMappings[deviceIndex];
		uint16_t virtualEndAddress = deviceMapping.m_virtualBusOffset + deviceMapping.m_pDevice->GetSize();
		// If the start or end of this device's virtual address space lands in the range, this device IS in range
		if ((deviceMapping.m_virtualBusOffset >= startAddr && deviceMapping.m_virtualBusOffset <= endAddr) ||
			(virtualEndAddress > startAddr && virtualEndAddress < endAddr))
		{
			return true;
		}
		deviceIndex++;
	}
	return false;
}

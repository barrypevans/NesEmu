#pragma once
#include "IMemoryDevice.h"
#include "INesFile.h"

class MapperPPUInterface;
class MapperCPUInterface;

class IMapper
{
public:
	IMapper(INesHeader header)
	{
		m_pPpuInterface = new MapperPPUInterface(this);
		m_pCpuInterface = new MapperCPUInterface(this);
	}
	~IMapper()
	{
		delete(m_pPpuInterface);
		delete(m_pCpuInterface);
	}

	virtual uint8_t PpuRead(uint16_t addr) = 0;
	virtual bool PpuWrite(uint16_t addr, uint8_t data) = 0;
	virtual uint8_t CpuRead(uint16_t addr) = 0;
	virtual bool CpuWrite(uint16_t addr, uint8_t data) = 0;

	uint8_t numPrgBanks = 0;
	uint8_t numChrBanks = 0;
	uint8_t* m_pPrgMemory;
	uint8_t* m_pChrMemory;

	MapperPPUInterface* m_pPpuInterface;
	MapperCPUInterface* m_pCpuInterface;
};

class MapperPPUInterface : public IMemoryDevice
{
public:
	IMapper* m_pMapper;
	MapperPPUInterface(IMapper* pMapper) { m_pMapper = pMapper; }
	virtual uint8_t Read(uint16_t addr) override { return m_pMapper->PpuRead(addr); };
	virtual bool Write(uint16_t addr, uint8_t data) override { return m_pMapper->PpuWrite(addr, data); };
	virtual uint16_t GetSize() override { return 0x2000; /*0x0000 -> 0x2000*/}
	virtual bool UseVirtualAddressSpace() override { return false; }
};

class MapperCPUInterface : public IMemoryDevice
{
public:
	IMapper* m_pMapper;
	MapperCPUInterface(IMapper* pMapper) { m_pMapper = pMapper; }
	virtual uint8_t Read(uint16_t addr) override { return m_pMapper->CpuRead(addr); };
	virtual bool Write(uint16_t addr, uint8_t data) override { return m_pMapper->CpuWrite(addr, data); };
	virtual uint16_t GetSize() override { return 0xBFDF; /*0x4020 -> 0xFFFF*/ }
	virtual bool UseVirtualAddressSpace() override { return false; }
};
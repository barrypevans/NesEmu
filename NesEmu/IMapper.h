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
		m_iNesHeader = header;
	}

	virtual bool PpuRemap(uint16_t addr, uint16_t& remappedAddr) = 0;
	virtual bool CpuRemap(uint16_t addr, uint16_t& remappedAddr) = 0;

private:
	INesHeader m_iNesHeader;
};


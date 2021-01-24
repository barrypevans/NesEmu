#pragma once
#include "IMemoryDevice.h"
#include "INesFile.h"

class IMapper
{
public:
	IMapper(INes::INesHeader header)
	{
		m_iNesHeader = header;
	}

	virtual bool PpuRemap(uint16_t addr, uint16_t& remappedAddr) = 0;
	virtual bool CpuRemap(uint16_t addr, uint16_t& remappedAddr) = 0;

protected:
	INes::INesHeader m_iNesHeader;
};


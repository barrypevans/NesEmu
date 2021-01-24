#include "Mapper000.h"



Mapper000::Mapper000(INes::INesHeader header) : IMapper(header)
{
}


Mapper000::~Mapper000()
{
}

bool Mapper000::CpuRemap(uint16_t addr, uint16_t & remappedAddr)
{
	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		// either mirror the memory twice of four times base on how many bands there are.
		remappedAddr = addr & (m_iNesHeader.numPrgChunks > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}
	return false;
}

bool Mapper000::PpuRemap(uint16_t addr, uint16_t & remappedAddr)
{
	if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		remappedAddr = addr;
		return true;
	}
	return false;
}

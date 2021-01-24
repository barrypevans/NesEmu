#pragma once
#include "IMapper.h"

class Mapper000 : public IMapper
{
public:
	Mapper000(INes::INesHeader header);
	~Mapper000();

	// Inherited via IMapper
	virtual bool PpuRemap(uint16_t addr, uint16_t & remappedAddr) override;
	virtual bool CpuRemap(uint16_t addr, uint16_t & remappedAddr) override;
};


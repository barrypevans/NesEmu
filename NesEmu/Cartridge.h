#pragma once
#include "Ram.h"
#include "Bus.h"
#include "IMapper.h"

#include <string>
#include <stdint.h>

class Cartridge
{
public:
	Cartridge(std::string romPath, Bus* m_pCpuBus, Bus* m_pPpuBus);
	~Cartridge();
	
	bool IsCatridgeReady() { return m_cartridgeReady; }
private:

	bool m_cartridgeReady = false;
	IMapper* m_pMapper;
	INesHeader m_iNesHeader;
};


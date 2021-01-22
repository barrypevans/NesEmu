#pragma once
#include "Bus.h"

class Ppu2C02
{
public:
	Ppu2C02(Bus* m_pPpuBus, Bus* m_pCpuBus);
	~Ppu2C02();

	Bus* m_pPpuBus;
	Bus* m_pCpuBus;
};


#include "ppu2C02.h"

Ppu2C02::Ppu2C02(Bus * pPpuBus, Bus * pCpuBus)
{
	m_pPpuBus = pPpuBus;
	m_pCpuBus = pCpuBus;
}

Ppu2C02::~Ppu2C02()
{
}

void Ppu2C02::Tick()
{
}

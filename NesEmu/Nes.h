#pragma once
#include "Bus.h"
#include "cpu6502.h"
#include "ppu2C02.h"
#include "Ram.h"

class Nes
{
public:
	Nes();
	~Nes();
	void Tick();
	void Reset();
	void UploadProgram(uint8_t* bin, uint16_t size);

	Bus* m_pCpuBus;
	Ram* m_pCpuRam;
	Ram* m_pProgamMemory;
	Cpu6502* m_pCpu;

	Bus* m_pPpuBus;
	Ram* m_pNameTableRam;
	Ram* m_pPaletteRam;
	Ppu2C02* m_pPpu;

	uint16_t m_programSize;
};


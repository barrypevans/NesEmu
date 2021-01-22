#pragma once
#include "Bus.h"
#include "cpu6502.h"
#include "Ram.h"

class Nes
{
public:
	Nes();
	~Nes();
	void Tick();
	void Reset();
	void UploadProgram(uint8_t* bin, uint16_t size);

	Bus* m_pBus;
	Ram* m_pRam;
	Ram* m_pProgamMemory;
	Cpu6502* m_pCpu;
	uint16_t m_programSize;
};


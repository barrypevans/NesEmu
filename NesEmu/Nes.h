#pragma once
#include "Bus.h"
#include "cpu6502.h"
#include "ppu2C02.h"
#include "Ram.h"
#include "Cartridge.h"

class Nes
{
public:
	Nes();
	~Nes();
	void Tick();
	void Reset();
	//void UploadProgram(uint8_t* bin, uint16_t size);
	void InsertCartridge(std::string romPath);
	void RemoveCartridge();
	bool HasCartridge() { return m_pCart != nullptr; }

	Bus* m_pCpuBus;
	Ram* m_pCpuRam;
	Cpu6502* m_pCpu;

	Bus* m_pPpuBus;
	Ram* m_pNameTableRam;
	Ram* m_pPaletteRam;
	Ppu2C02* m_pPpu;

	Cartridge* m_pCart = nullptr;

	uint64_t m_tickCount;

};


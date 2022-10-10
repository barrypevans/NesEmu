#pragma once
#include "Bus.h"
#include "cpu6502.h"
#include "ppu2C02.h"
#include "Ram.h"
#include "Cartridge.h"
#include "IOBusInterface.h"

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

	void StartDma() { m_dmaActive = true; }

	Bus* m_pCpuBus;
	Ram* m_pCpuRam;
	Cpu6502* m_pCpu;
	IOBusInterface* m_pIOBusInterface;

	Bus* m_pPpuBus;
	Ppu2C02::PPUNameTableInterface* m_pNameTableRam;
	uint8_t* m_pPaletteRam;
	Ppu2C02* m_pPpu;

	Cartridge* m_pCart = nullptr;

	uint64_t m_tickCount;

	

	class PaletteRamInterface : public IMemoryDevice
	{
		// Inherited via IMemoryDevice
		virtual uint8_t Read(uint16_t addr) override;
		virtual bool Write(uint16_t addr, uint8_t data) override;
		virtual uint16_t GetSize() override;
		virtual bool UseVirtualAddressSpace() override;
		uint8_t* m_pData;
	public:
		void SetMemory(uint8_t* data) { m_pData = data; };
	};
	PaletteRamInterface* m_pPaletteRamInterface;


	// DMA
	uint8_t m_dmaPage = 0;
	uint8_t m_dmaAddr = 0;
	uint8_t m_dmaData = 0;
	bool m_dmaActive = false;
	bool m_dmaWaitForOddCycle = false;
};


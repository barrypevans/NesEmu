#pragma once
#include "Ram.h"
#include "Bus.h"
#include "IMapper.h"

#include <string>
#include <stdint.h>

class CartridgePPUInterface;
class CartridgeCPUInterface;

class Cartridge
{
public:
	Cartridge(std::string romPath, Bus* m_pCpuBus, Bus* m_pPpuBus);
	~Cartridge();
	
	uint8_t CpuRead(uint16_t addr);
	bool CpuWrite(uint16_t addr, uint8_t data);
	uint8_t PpuRead(uint16_t addr);
	bool PpuWrite(uint16_t addr, uint8_t data);

	bool IsCatridgeReady() { return m_cartridgeReady; }
	
	enum MirrorMode
	{
		kMirrorModeHorizontal,
		kMirrorModeVertical,
		kMirrorModeOneScreenLo,
		kMirrorModeOneScreenHi
	}m_mirrorMode = kMirrorModeHorizontal;

private:

	bool InitMapper();

	uint8_t numPrgBanks = 0;
	uint8_t numChrBanks = 0;
	uint8_t* m_pPrgMemory = nullptr;
	uint8_t* m_pChrMemory = nullptr;
	uint8_t m_mapperId;

	bool m_cartridgeReady = false;
	IMapper* m_pMapper = nullptr;
	INes::INesHeader m_iNesHeader;

	CartridgePPUInterface* m_pPpuInterface = nullptr;
	CartridgeCPUInterface* m_pCpuInterface = nullptr;

	Bus * m_pCpuBus;
	Bus * m_pPpuBus;
};

class CartridgePPUInterface : public IMemoryDevice
{
public:
	Cartridge* m_pCart;
	CartridgePPUInterface(Cartridge* pCart) { m_pCart = pCart; }
	virtual uint8_t Read(uint16_t addr) override { return m_pCart->PpuRead(addr); }
	virtual bool Write(uint16_t addr, uint8_t data) override { return m_pCart->PpuWrite(addr, data); }
	virtual uint16_t GetSize() override { return 0x2000; /*0x0000 -> 0x2000*/ }
	virtual bool UseVirtualAddressSpace() override { return false; }
};

class CartridgeCPUInterface : public IMemoryDevice
{
public:
	Cartridge* m_pCart;
	CartridgeCPUInterface(Cartridge* pCart) { m_pCart = pCart; }
	virtual uint8_t Read(uint16_t addr) override { return m_pCart->CpuRead(addr); }
	virtual bool Write(uint16_t addr, uint8_t data) override { return m_pCart->CpuWrite(addr, data); }
	virtual uint16_t GetSize() override { return 0xBFE0; /*0x4020 -> 0xFFFF*/ }
	virtual bool UseVirtualAddressSpace() override { return false; }
};
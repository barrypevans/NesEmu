#include "Nes.h"

Nes::Nes()
{
	m_pCpuBus = new Bus(); 
	m_pPpuBus = new Bus();
	
	// hook cpu and ppu up to buses
	m_pCpu = new Cpu6502(m_pCpuBus, m_pPpuBus);
	m_pPpu = new Ppu2C02(m_pPpuBus, m_pCpuBus);
	m_pIOBusInterface = new IOBusInterface();

	m_pCpuRam = new Ram(0x0800);
	// first 2k of ram mirror across the first 8k of address bus
	m_pCpuBus->RegisterMemoryDeviceMirror(m_pCpuRam, 0x0000, 3);
	m_pCpuBus->RegisterMemoryDeviceMirror(m_pPpu->m_pRegisterInterface, 0x2000, 400);
	m_pCpuBus->RegisterMemoryDevice(m_pIOBusInterface, 0x4000, 0x4017);
	

	// hook up on board palette memory
	m_pPaletteRam = (uint8_t*)malloc(0x0020);
	m_pPaletteRamInterface = new PaletteRamInterface();
	m_pPaletteRamInterface->SetMemory(m_pPaletteRam);

	// hook up on board ppu ram
	m_pNameTableRam = new Ppu2C02::PPUNameTableInterface(0x0800, m_pPpu);
	m_pPpuBus->RegisterMemoryDevice(m_pNameTableRam, 0x2000, 0x2FFF);
	m_pPpuBus->RegisterMemoryDevice(m_pNameTableRam, 0x3000, 0x3EFF); // 4 1k name tables are mirrored
	m_pPpuBus->RegisterMemoryDevice(m_pPaletteRamInterface, 0x3F00);

	m_tickCount = 0;
}

Nes::~Nes()
{
	delete m_pCpu;
	delete m_pCpuRam;
	delete m_pCpuBus;
	free(m_pPaletteRam);
}

void Nes::Tick()
{
	// check if dma was initiated last cycle
	if (m_pIOBusInterface->m_dmaShouldStart)
	{
		m_pIOBusInterface->m_dmaShouldStart = false;
		m_dmaPage = m_pIOBusInterface->m_dmaPage;
		m_dmaActive = true;
		m_dmaAddr = 0;
	}

	m_pPpu->Tick();
	if ((m_tickCount % 3) == 0)
	{
		if (!m_dmaActive)
		{
			m_pCpu->Tick();
		}
		else 
		{
			bool isOdd = m_tickCount % 2 == 1;
			// dma is synced to odd cpu clock cycles.... so wait one cycle if the current cycle is odd
			if (!m_dmaWaitForOddCycle && isOdd)
			{
				m_dmaWaitForOddCycle = true;
			}
			else if(m_dmaWaitForOddCycle)
			{
				if (!isOdd)
				{
					// Even cycle, read a byte fromt cpu ram
					uint16_t cpuAddr = (((uint16_t)m_dmaPage) << 8) | (uint16_t)m_dmaAddr;
					m_dmaData = m_pCpuBus->Read(cpuAddr);
				}
				else
				{
					// Odd cycle, write the byoe we read last cycle to oam ram directly
					m_pPpu->m_pOamRaw[m_dmaAddr] = m_dmaData;
					m_dmaAddr++;

					if (m_dmaAddr == 0x00) // when dma addr overflows to 0x00, we know we have writen all 256 bytes of oam ram
					{
						m_dmaWaitForOddCycle = false;
						m_dmaActive = false;
					}
				}
			}
		}
	}
	
	if (m_pPpu->m_nmi == true)
	{
		m_pPpu->m_nmi = false;
		m_pCpu->Nmi();
	}

	m_tickCount++;
}

void Nes::Reset()
{
	m_pCpu->Reset();
}

void Nes::InsertCartridge(std::string romPath)
{
	if (m_pCart) RemoveCartridge();

	m_pCart = new Cartridge(romPath, m_pCpuBus, m_pPpuBus);

	// If cartrigfe fails to load, remove it.
	if (!m_pCart->IsCatridgeReady())
	{
		RemoveCartridge();
	}
	else
	{
		m_pPpu->SetMirrorMode(m_pCart->m_mirrorMode);
	}
	m_pCpu->Reset();

}

void Nes::RemoveCartridge()
{
	if (m_pCart) delete m_pCart;
	m_pCart = nullptr;
}


uint8_t Nes::PaletteRamInterface::Read(uint16_t addr)
{
	if ((addr % 0x0004) == 0)
		return m_pData[0];
	
	return m_pData[addr];
}

bool Nes::PaletteRamInterface::Write(uint16_t addr, uint8_t data)
{
	addr &= 0x001F;
	if (addr == 0x0010) addr = 0x0000;
	if (addr == 0x0014) addr = 0x0004;
	if (addr == 0x0018) addr = 0x0008;
	if (addr == 0x001C) addr = 0x000C;
	m_pData[addr] = data;
	return true;
}

uint16_t Nes::PaletteRamInterface::GetSize()
{
	return 0x0020;
}

bool Nes::PaletteRamInterface::UseVirtualAddressSpace()
{
	return true;
}

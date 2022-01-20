#include "ppu2C02.h"

Ppu2C02::Ppu2C02(Bus * pPpuBus, Bus * pCpuBus)
{
	m_pPpuBus = pPpuBus;
	m_pRegisterInterface = new PPURegisterInterface();
	m_pRegisterInterface->SetPpu(this);
}

Ppu2C02::~Ppu2C02()
{
	delete m_pRegisterInterface;
}

void Ppu2C02::Tick()
{
	m_frameCompleted = false;
	m_cycle++;
	if (m_cycle >= 341)
	{
		m_cycle = 0;
		m_scanline++;
		if (m_scanline >= 261)
		{
			m_scanline = 0;
			m_frameCompleted = true;
		}
	}
}


uint8_t Ppu2C02::PPURegisterInterface::Read(uint16_t addr)
{
	if (addr == STATUS_REG)
	{
		// only care about the top three bits of the status register
		// the bottom bits are usually noise, or whatever was last in the data register...🤷
		uint8_t data = (m_pPpu->m_status & 0xE0) | (m_pPpu->m_data & 0x1F); 

		// reading the status register always sets the vblank flag to 0
		m_pPpu->m_verticalBlank = 0;
		m_pPpu->m_addrLatch = 0;
		return data;
	}

	return m_pPpu->m_registers[addr];
}

bool Ppu2C02::PPURegisterInterface::Write(uint16_t addr, uint8_t data)
{
	if (addr == ADDR_REG)
	{
		// alternatively buffer upper and lower bytes
		if (m_pPpu->m_addrLatch == 0)
		{
			m_pPpu->m_bufferedAddr = (m_pPpu->m_addr & 0x00FF) | (((uint16_t)data) << 1);
			m_pPpu->m_addrLatch = 1;
		}
		else
		{
			m_pPpu->m_bufferedAddr = (m_pPpu->m_addr & 0xFF00) | data;
			m_pPpu->m_addrLatch = 0;
			
			if (m_pPpu->m_bufferedAddr > 0x03F00) // reading palette memory instantiously
			{
				m_pPpu->m_data = m_pPpu->m_pPpuBus->Read(m_pPpu->m_bufferedAddr);
			}
		}
	}

	if(addr != STATUS_REG) // cant write to the status register from cpu
		m_pPpu->m_registers[addr] = data;
	
	return true;
}

uint16_t Ppu2C02::PPURegisterInterface::GetSize()
{
	return 8;
}

bool Ppu2C02::PPURegisterInterface::UseVirtualAddressSpace()
{
	return true;
}

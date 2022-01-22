#include "ppu2C02.h"

Ppu2C02::Ppu2C02(Bus* pPpuBus, Bus* pCpuBus)
{
	m_pPpuBus = pPpuBus;
	m_pRegisterInterface = new PPURegisterInterface();
	m_pRegisterInterface->SetPpu(this);
	memset(m_registers, 0, 8);
	m_enableNmi = false;
}

Ppu2C02::~Ppu2C02()
{
	delete m_pRegisterInterface;
}

void Ppu2C02::Tick()
{

	if (m_cycle == 1 && m_scanline == -1)
	{
		m_verticalBlank = 0;
	}

	if (m_cycle == 1 && m_scanline == 241)
	{
		m_verticalBlank = 1;
		if (m_enableNmi)
			m_nmi = true;
	}

	m_frameCompleted = false;
	m_cycle++;
	if (m_cycle >= 341)
	{
		m_cycle = 0;
		m_scanline++;
		if (m_scanline >= 261)
		{
			m_scanline = -1;
			m_frameCompleted = true;
		}
	}


}

void Ppu2C02::GetPatternTable(uint32_t* pData, uint8_t index, uint8_t palette)
{
	uint16_t tableOffset = index * 0x1000;
	for (uint8_t tile_x = 0; tile_x < 16; ++tile_x)
	{
		for (uint8_t tile_y = 0; tile_y < 16; ++tile_y)
		{
			// tile_y * 256 wher 256 = (16 tiles in a row * (8 bytes per tile plane * 2 bit planes))
			uint16_t byteOffset = tile_y * 256 + tile_x * 16;
			for (uint8_t pixel_x = 0; pixel_x < 8; ++pixel_x)
			{
				uint16_t baseAddr = tableOffset + byteOffset + pixel_x;
				uint8_t tileLSB = m_pPpuBus->Read(baseAddr);
				uint8_t tileMSB = m_pPpuBus->Read(baseAddr + 8);
				for (uint8_t pixel_y = 0; pixel_y < 8; ++pixel_y)
				{
					uint8_t paletteIndex = (tileLSB & 0x01) << 1 | (tileMSB & 0x01);
					tileLSB >>= 1;
					tileMSB >>= 1;
					
					uint16_t pixelIndexX = tile_x * 8 + (7 - pixel_y);
					uint16_t pixelIndexY = tile_y * 8 + pixel_x;
					uint16_t pixelIndex = pixelIndexX + pixelIndexY * 128;
					uint32_t color = GetColorFromPalette(paletteIndex, palette);
					pData[pixelIndex] = ((color & 0x0000FFFF) << 16) | ((color & 0xFFFF0000) >> 16);
				}
			}
		}
	}
}

uint32_t Ppu2C02::GetColorFromPalette(uint8_t index, uint8_t palette)
{
	uint16_t addr = 0x3F00 + index + palette * 4;
	uint8_t colorIndex = m_pPpuBus->Read(addr) & 0x3F;
	return m_paletteColors[colorIndex] ;
}

uint8_t Ppu2C02::PPURegisterInterface::Read(uint16_t addr)
{
	if (addr == STATUS_REG)
	{
		// only care about the top three bits of the status register
		// the bottom bits are usually noise, or whatever was last in the data register...🤷
		uint8_t data = (m_pPpu->m_status & 0xE0) | (m_pPpu->m_bufferedData & 0x1F);

		// reading the status register always sets the vblank flag to 0
		m_pPpu->m_verticalBlank = 0;
		m_pPpu->m_addrLatch = 0;
		return data;
	}
	
	if (addr == DATA_REG)// auto increment addr on read and write
	{
		uint8_t outputData = m_pPpu->m_bufferedData;
		m_pPpu->m_bufferedData = m_pPpu->m_pPpuBus->Read(m_pPpu->m_bufferedAddr);
		if (m_pPpu->m_bufferedAddr >= 0x03F00) // reading palette memory instantiously
		{
			outputData = m_pPpu->m_bufferedData;
		}
		m_pPpu->m_bufferedAddr++;
		
		return outputData;
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
			m_pPpu->m_addr = data;
			m_pPpu->m_addrLatch = 1;
		}
		else
		{
			m_pPpu->m_bufferedAddr = ((uint16_t)m_pPpu->m_addr << 8) | (uint16_t)data;
			m_pPpu->m_addr = data;
			m_pPpu->m_addrLatch = 0;
		}
	}
	else if (addr == DATA_REG)
	{
		m_pPpu->m_pPpuBus->Write(m_pPpu->m_bufferedAddr, data);
		m_pPpu->m_bufferedAddr++;
	}
	else if (addr != STATUS_REG) // cant write to the status register from cpu
	{
		m_pPpu->m_registers[addr] = data;
	}
	else
	{
		return false;
	}
	
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


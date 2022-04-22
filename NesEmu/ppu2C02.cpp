#include "ppu2C02.h"

/****************************************************************************/

Ppu2C02::Ppu2C02(Bus* pPpuBus, Bus* pCpuBus)
{
	m_pPpuBus = pPpuBus;
	m_pRegisterInterface = new PPURegisterInterface();
	m_pRegisterInterface->SetPpu(this);

	memset(m_registers, 0, 8);

	m_cycle = 0;
	m_scanline = -1;

	m_enableNmi = false;
	m_vramAddr.m_addr = 0;
	m_tramAddr.m_addr = 0;
	m_fineX = 0;

	m_internalTileIndex = 0;
	m_internalAttributeData = 0;
	m_internalPlaneLSB = 0;
	m_internalPlaneMSB = 0;

	m_interalShiftLSB = 0;
	m_interalShiftMSB = 0;
	m_interalShiftAttribLSB = 0;
	m_interalShiftAttribMSB = 0;
}

Ppu2C02::~Ppu2C02()
{
	delete m_pRegisterInterface;
}

/****************************************************************************/

void Ppu2C02::Internal_SetShiftRegisters()
{
	m_interalShiftLSB = (m_interalShiftLSB & 0xFF00) | m_internalPlaneLSB;
	m_interalShiftMSB = (m_interalShiftMSB & 0xFF00) | m_internalPlaneMSB;
	m_interalShiftAttribLSB = (m_interalShiftAttribLSB & 0xFF00) | ((m_internalAttributeData & 0x01) ? 0xFF : 0x00); // fill the attribute shifter bits with this tiles pallet index
	m_interalShiftAttribMSB = (m_interalShiftAttribMSB & 0xFF00) | ((m_internalAttributeData & 0x02) ? 0xFF : 0x00);
}

void Ppu2C02::Internal_UpdateShiftRegisters()
{
	if (!m_showBackground)return;

	m_interalShiftLSB <<= 1;
	m_interalShiftMSB <<= 1;
	m_interalShiftAttribLSB <<= 1;
	m_interalShiftAttribMSB <<= 1;
}


void Ppu2C02::Tick()
{
	bool canRender = m_showBackground || m_showSprites;


	if (m_scanline >= -1 && m_scanline < 240 && m_cycle != 0)
	{

		if (m_cycle == 1 && m_scanline == -1) // clear vblank
		{
			m_verticalBlank = 0;
		}

		if ((m_cycle >= 1 && m_cycle <= 256) || (m_cycle >= 321 && m_cycle < 337)) // do work!
		{
			Internal_UpdateShiftRegisters();
			uint8_t modCycle = m_cycle & 0x07;
			switch (modCycle)
			{
			case kPpuPhaseNTRead:
			{
				Internal_SetShiftRegisters(); // beginning of the tile, buffer last tile shifters
				uint16_t ntOffset = 0x2000; // base name table address
				ntOffset = ntOffset | (m_vramAddr.m_addr & 0x0FFF); // coarse x, coarse y, nt x, nt y - get the nametable and tile
				m_internalTileIndex = m_pPpuBus->Read(ntOffset);
				break;
			}
			case kPpuPhaseATRead:
			{
				uint16_t atOffset = (((uint16_t)m_vramAddr.m_coarseX) >> 2) | ((((uint16_t)m_vramAddr.m_coarseY ) << 1) & 0xF8); // the top 3 coarse bits are the same as the at indices
				atOffset = atOffset | (((uint16_t)m_vramAddr.m_ntx) << 10) | (((uint16_t)m_vramAddr.m_nty) << 11); // 11th and 12th bit control nametables as they start at 23c0, 27c0, 2bc0 and 2fc0
				atOffset = 0x23C0 | atOffset; // base address is 23c0
				m_internalAttributeData = m_pPpuBus->Read(atOffset);

				// extract the two bit data. we know which of the groups in the quad we are looking at so lets choose the appropriate attr data for this tile
				if (m_vramAddr.m_coarseX & 0x02)
					m_internalAttributeData >>= 2;
				if (m_vramAddr.m_coarseY & 0x02)
					m_internalAttributeData >>= 4;
				m_internalAttributeData &= 0x03;
				break;
			}
			case kPpuPhaseBGTileLow:
			{
				uint16_t tableOffset = ((uint16_t) m_bgPatternTableIndex) << 12; // pattern table 1 starts at 0x0000, pattern table 2 starts at 0x1000
				uint16_t tileByteOffset = m_internalTileIndex * 16; // each tile is two 8 byte planes, so multiply by 16 to get the correct byte offset.
				uint16_t tileaddr = tableOffset + tileByteOffset + m_vramAddr.m_fineY; // row is 8 bits in a plane so just add fine y to increment row.
				m_internalPlaneLSB = m_pPpuBus->Read(tileaddr);
				break;
			}
			case kPpuPhaseBGTileHigh:
			{
				uint16_t tableOffset = ((uint16_t)m_bgPatternTableIndex) << 12; // pattern table 1 starts at 0x0000, pattern table 2 starts at 0x1000
				uint16_t tileByteOffset = m_internalTileIndex * 16; // each tile is two 8 byte planes, so multiply by 16 to get the correct byte offset.
				uint16_t tileaddr = tableOffset + tileByteOffset + m_vramAddr.m_fineY + 8; // row is 8 bits in a plane so just add fine y to increment row. add 8 bytes to get to the second bit plane
				m_internalPlaneMSB = m_pPpuBus->Read(tileaddr);
				break;
			}
			case kPpuPhaseBGTileComplete:
			{
				if (canRender) // check that rendering mask not set
				{
					// move to the next tile
					if (m_vramAddr.m_coarseX >= 31)// at the end of the nametable, coarse x wraps and nametable increments
					{
						m_vramAddr.m_coarseX = 0;
						m_vramAddr.m_ntx = ~m_vramAddr.m_ntx;
					}
					else
					{
						m_vramAddr.m_coarseX++;
					}
				}
				break;
			}
			}
		}

		if (m_cycle == 256 && canRender) // reset horizontal for next scanline
		{
			// move to next scanline
			if (m_vramAddr.m_fineY >= 7)
			{
				m_vramAddr.m_fineY = 0;
				if (m_vramAddr.m_coarseY == 29) // we are in the name table here so swap nametables as expected
				{
					m_vramAddr.m_coarseY = 0;
					m_vramAddr.m_nty = ~m_vramAddr.m_nty;
				}
				else if (m_vramAddr.m_coarseY == 31) // here, were in the attribute table so just jump to the top of the current name table... not sure why we would ever get here
				{
					m_vramAddr.m_coarseY = 0;
				}
				else
				{
					m_vramAddr.m_coarseY++;
				}
			}
			else
			{
				m_vramAddr.m_fineY++;
			}
		}

		if (m_cycle == 257 && canRender) // reset horizontal for next scanline
		{
			Internal_SetShiftRegisters();
			m_vramAddr.m_coarseX = m_tramAddr.m_coarseX;
			m_vramAddr.m_ntx = m_tramAddr.m_ntx;
		}

		if ((m_cycle >= 280 && m_cycle <= 304) && m_scanline == -1 && canRender) // at the beginng of a new frame, reset y values
		{
			m_vramAddr.m_coarseY = m_tramAddr.m_coarseY;
			m_vramAddr.m_nty = m_tramAddr.m_nty;
			m_vramAddr.m_fineY = m_tramAddr.m_fineY;
		}
	}

	// put pixels on the screeen!!!
	if (m_showBackground)
	{
		uint16_t shifterFlag = 0x8000 >> m_fineX;
		uint8_t lsb = (shifterFlag & m_interalShiftLSB) != 0;
		uint8_t msb = (shifterFlag & m_interalShiftMSB) != 0;
		uint8_t attrLsb = (shifterFlag & m_interalShiftAttribLSB) != 0;
		uint8_t attrMsb = (shifterFlag & m_interalShiftAttribMSB) != 0;
		uint8_t pixel = (msb & 0x01) << 1 | (lsb & 0x01);
		uint8_t pallete = (attrMsb & 0x01) << 1 | (attrLsb & 0x01);
		m_bufferedPixel = GetColorFromPalette(pixel, pallete);
	}

	if (m_cycle == 1 && m_scanline == 241) // start vblank
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
					pData[pixelIndex] = GetColorFromPalette(paletteIndex, palette);
				}
			}
		}
	}
}

void Ppu2C02::GetNameTable(uint32_t* pData, uint8_t index, uint8_t palette)
{
	uint16_t patternTableOffset = 0;
	for (uint8_t tile_x = 0; tile_x < 32; ++tile_x)
	{
		for (uint8_t tile_y = 0; tile_y < 30; ++tile_y)
		{
			uint16_t nameTableAddr = 0x2000 | (tile_x + tile_y * 32);
			nameTableAddr |= 3096 * index;
			uint8_t tileIndex = m_pPpuBus->Read(nameTableAddr);
			uint16_t byteOffset = tileIndex * 16;
			for (uint8_t pixel_x = 0; pixel_x < 8; ++pixel_x)
			{
				uint16_t baseAddr = patternTableOffset + byteOffset + pixel_x;
				uint8_t tileLSB = m_pPpuBus->Read(baseAddr);
				uint8_t tileMSB = m_pPpuBus->Read(baseAddr + 8);
				for (uint8_t pixel_y = 0; pixel_y < 8; ++pixel_y)
				{
					uint8_t paletteIndex = (tileLSB & 0x01) << 1 | (tileMSB & 0x01);
					tileLSB >>= 1;
					tileMSB >>= 1;

					uint16_t pixelIndexX = tile_x * 8 + (7 - pixel_y);
					uint16_t pixelIndexY = tile_y * 8 + pixel_x;
					uint16_t pixelIndex = pixelIndexX + pixelIndexY * 256;
					pData[pixelIndex] = GetColorFromPalette(paletteIndex, palette);
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

/****************************************************************************/

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
		m_pPpu->m_bufferedData = m_pPpu->m_pPpuBus->Read(m_pPpu->m_vramAddr.m_addr);
		if (m_pPpu->m_vramAddr.m_addr >= 0x03F00) // reading palette memory instantiously
		{
			outputData = m_pPpu->m_bufferedData;
		}
		m_pPpu->m_vramAddr.m_addr += (m_pPpu->m_incrementMode ? 32 : 1);
		return outputData;
	}

	return 0x00;
}

bool Ppu2C02::PPURegisterInterface::Write(uint16_t addr, uint8_t data)
{
	if (addr == ADDR_REG)
	{
		// alternatively buffer upper and lower bytes
		if (m_pPpu->m_addrLatch == 0)
		{
			m_pPpu->m_tramAddr.m_addr = ((uint16_t)m_pPpu->m_tramAddr.m_addr & 0x00FF) | ((uint16_t)(data & 0x3F) << 8);
			m_pPpu->m_addrLatch = 1;
		}
		else
		{
			m_pPpu->m_tramAddr.m_addr = ((uint16_t)m_pPpu->m_tramAddr.m_addr & 0xFF00) | (uint16_t)data;
			m_pPpu->m_vramAddr.m_addr = m_pPpu->m_tramAddr.m_addr;
			m_pPpu->m_addrLatch = 0;
		}
	}
	else if (addr == SCROLL_REG)
	{
		// alternatively buffer upper and lower bytes
		if (m_pPpu->m_addrLatch == 0)
		{
			m_pPpu->m_fineX = data & 0x07; // bottom two bits are the pixel within the tile
			m_pPpu->m_tramAddr.m_coarseX = data >> 3; // top 5 bits are the tile within the table
			m_pPpu->m_addrLatch = 1;
		}
		else
		{
			m_pPpu->m_tramAddr.m_fineY = data & 0x07; // bottom two bits are the pixel within the tile
			m_pPpu->m_tramAddr.m_coarseY = data >> 3; // top 5 bits are the tile within the table
			m_pPpu->m_addrLatch = 0;
		}
	}
	else if (addr == CONTROL_REG)
	{
		// control register controls which nametable to read/write
		m_pPpu->m_control = data;
		m_pPpu->m_tramAddr.m_ntx = m_pPpu->m_nameTableX & 0x01;
		m_pPpu->m_tramAddr.m_nty = m_pPpu->m_nameTableY & 0x02;
	}
	else if (addr == DATA_REG)
	{
		m_pPpu->m_pPpuBus->Write(m_pPpu->m_vramAddr.m_addr, data);
		m_pPpu->m_vramAddr.m_addr += (m_pPpu->m_incrementMode ? 32: 1);
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

/****************************************************************************/

uint8_t Ppu2C02::PPUNameTableInterface::Read(uint16_t addr)
{

	return m_data[DoMirroring(addr)];
}

bool Ppu2C02::PPUNameTableInterface::Write(uint16_t addr, uint8_t data)
{
	m_data[DoMirroring(addr)] = data;
	return true;
}

uint16_t Ppu2C02::PPUNameTableInterface::GetSize()
{
	return m_size;
}

bool Ppu2C02::PPUNameTableInterface::UseVirtualAddressSpace()
{
	return true;
}

Ppu2C02::PPUNameTableInterface::PPUNameTableInterface(uint16_t size, Ppu2C02* pPpu)
{
	m_size = size;
	m_pPpu = pPpu;
	m_data = (uint8_t*)malloc(size);
	memset(m_data, 0x00, size);
}

Ppu2C02::PPUNameTableInterface::~PPUNameTableInterface()
{
	free(m_data);
}


uint16_t Ppu2C02::PPUNameTableInterface::DoMirroring(uint16_t addr)
{
	uint16_t mirroredAddr = addr;
	if (m_pPpu->GetMirrorMode() == Cartridge::kMirrorModeHorizontal)
	{
		if (addr >= 0x0000 && addr <= 0x03FF)
			mirroredAddr = addr & 0x03FF;
		if (addr >= 0x0400 && addr <= 0x07FF)
			mirroredAddr = addr & 0x03FF;
		if (addr >= 0x0800 && addr <= 0x0BFF)
			mirroredAddr = (addr & 0x03FF) + 0x400;
		if (addr >= 0x0C00 && addr <= 0x0FFF)
			mirroredAddr = (addr & 0x03FF) + 0x400;
	}
	else if (m_pPpu->GetMirrorMode() == Cartridge::kMirrorModeVertical)
	{
		if (addr >= 0x0000 && addr <= 0x03FF)
			mirroredAddr = addr & 0x03FF;
		if (addr >= 0x0400 && addr <= 0x07FF)
			mirroredAddr = (addr & 0x03FF) + 0x400;
		if (addr >= 0x0800 && addr <= 0x0BFF)
			mirroredAddr = addr & 0x03FF;
		if (addr >= 0x0C00 && addr <= 0x0FFF)
			mirroredAddr = (addr & 0x03FF) + 0x400;
	}
	return mirroredAddr;
}

/****************************************************************************/
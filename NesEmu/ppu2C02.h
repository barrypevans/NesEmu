#pragma once
#include "Bus.h"

class Ppu2C02
{
public:
	Ppu2C02(Bus* m_pPpuBus, Bus* m_pCpuBus);
	~Ppu2C02();

	void Tick();
	void GetPatternTable(uint32_t* pData, uint8_t index, uint8_t palette);
	uint32_t GetColorFromPalette(uint8_t index, uint8_t palette);
	Bus* m_pPpuBus;
	Bus* m_pCpuBus;

	int16_t m_scanline = 0;
	int16_t m_cycle = 0;
	bool m_frameCompleted = false;

	class PPURegisterInterface : public IMemoryDevice
	{

		// Inherited via IMemoryDevice
		virtual uint8_t Read(uint16_t addr) override;
		virtual bool Write(uint16_t addr, uint8_t data) override;
		virtual uint16_t GetSize() override;
		virtual bool UseVirtualAddressSpace() override;
		Ppu2C02* m_pPpu;

	public:
		void SetPpu(Ppu2C02* ppu) { m_pPpu = ppu; };

	};
	PPURegisterInterface* m_pRegisterInterface;

	union
	{
		struct
		{
			union
			{
				struct
				{
					uint8_t m_1 : 1;
					uint8_t m_garbo : 4;
					uint8_t m_incrementMode : 1;
					uint8_t m_2 : 1;
					bool m_enableNmi : 1;
				};
				uint8_t m_control;
			};
			uint8_t m_mask;
			union
			{
				struct
				{
					uint8_t m_padding1 : 4;
					uint8_t m_padding2 : 1;
					uint8_t m_spriteOverflow : 1;
					uint8_t m_spriteHit : 1;
					uint8_t m_verticalBlank : 1;
				};
				uint8_t m_status;
			};
			uint8_t m_OamAddr;
			uint8_t m_OamData;
			uint8_t m_scroll;
			uint8_t m_addr;// not actually used
			uint8_t m_data;// not actually used
		};
		uint8_t m_registers[8];
	};

	enum
	{
		CONTROL_REG,
		MASK_REG,
		STATUS_REG,
		OAM_ADDR_REG,
		OAM_DATA_REG,
		SCROLL_REG,
		ADDR_REG,
		DATA_REG
	};

	uint8_t m_addrLatch = 0;
	uint8_t m_bufferedData = 0;
	uint16_t m_bufferedAddr = 0;

	bool m_nmi = false;

	uint32_t m_debugPalette[4] = {0x00000000, 0xFF00FF00, 0xFF0000FF, 0xFFFF0000};

	uint32_t m_paletteColors[56] = 
	{ ((84 << 16) | (84 << 8) | 84),
		((0 << 16) | (30 << 8) | 116),
		((8 << 16) | (16 << 8) | 144),
		((48 << 16) | (0 << 8) | 136),
		((68 << 16) | (0 << 8) | 100),
		((92 << 16) | (0 << 8) | 48),
		((84 << 16) | (4 << 8) | 0),
		((60 << 16) | (24 << 8) | 0),
		((32 << 16) | (42 << 8) | 0),
		((8 << 16) | (58 << 8) | 0),
		((0 << 16) | (64 << 8) | 0),
		((0 << 16) | (60 << 8) | 0),
		((0 << 16) | (50 << 8) | 60),
		((0 << 16) | (0 << 8) | 0),
		((152 << 16) | (150 << 8) | 152),
		((8 << 16) | (76 << 8) | 196),
		((48 << 16) | (50 << 8) | 236),
		((92 << 16) | (30 << 8) | 228),
		((136 << 16) | (20 << 8) | 176),
		((160 << 16) | (20 << 8) | 100),
		((152 << 16) | (34 << 8) | 32),
		((120 << 16) | (60 << 8) | 0),
		((84 << 16) | (90 << 8) | 0),
		((40 << 16) | (114 << 8) | 0),
		((8 << 16) | (124 << 8) | 0),
		((0 << 16) | (118 << 8) | 40),
		((0 << 16) | (102 << 8) | 120),
		((0 << 16) | (0 << 8) | 0),
		((236 << 16) | (238 << 8) | 236),
		((76 << 16) | (154 << 8) | 236),
		((120 << 16) | (124 << 8) | 236),
		((176 << 16) | (98 << 8) | 236),
		((228 << 16) | (84 << 8) | 236),
		((236 << 16) | (88 << 8) | 180),
		((236 << 16) | (106 << 8) | 100),
		((212 << 16) | (136 << 8) | 32),
		((160 << 16) | (170 << 8) | 0),
		((116 << 16) | (196 << 8) | 0),
		((76 << 16) | (208 << 8) | 32),
		((56 << 16) | (204 << 8) | 108),
		((56 << 16) | (180 << 8) | 204),
		((60 << 16) | (60 << 8) | 60),
		((236 << 16) | (238 << 8) | 236),
		((168 << 16) | (204 << 8) | 236),
		((188 << 16) | (188 << 8) | 236),
		((212 << 16) | (178 << 8) | 236),
		((236 << 16) | (174 << 8) | 236),
		((236 << 16) | (174 << 8) | 212),
		((236 << 16) | (180 << 8) | 176),
		((228 << 16) | (196 << 8) | 144),
		((204 << 16) | (210 << 8) | 120),
		((180 << 16) | (222 << 8) | 120),
		((168 << 16) | (226 << 8) | 144),
		((152 << 16) | (226 << 8) | 180),
		((160 << 16) | (214 << 8) | 228),
		((160 << 16) | (162 << 8) | 160) };

};


#pragma once
#include "Bus.h"

class Ppu2C02
{
public:
	Ppu2C02(Bus* m_pPpuBus, Bus* m_pCpuBus);
	~Ppu2C02();

	void Tick();
	void GetPatternTable(uint32_t* pData, uint8_t index);

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
		union
		{
			struct
			{
				bool m_enableNmi : 1;
				uint8_t m_1 : 1;
				uint8_t m_2 : 1;
				uint8_t m_garbo : 5;
			};
			uint8_t m_control;
		};
		uint8_t m_mask;
		union
		{
			struct
			{
				uint8_t m_verticalBlank : 1;
				uint8_t m_spriteHit : 1;
				uint8_t m_spriteOverflow : 1;
				uint8_t m_garbo : 5;
			};
			uint8_t m_status;
		};
		uint8_t m_OamAddr;
		uint8_t m_OamData;
		uint8_t m_scroll;
		uint8_t m_addr;
		uint8_t m_data;

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
	uint16_t m_bufferedAddr = 0;

	bool m_nmi;

	uint32_t m_debugPalette[4] = {0x00000000, 0xFF00FF00, 0xFF0000FF, 0xFFFF0000};

		/*84  84  84    0  30 116    8  16 144   48   0 136   68   0 100   92   0  48   84   4   0   60  24   0   32  42   0    8  58   0    0  64   0    0  60   0    0  50  60    0   0   0
		152 150 152    8  76 196   48  50 236   92  30 228  136  20 176  160  20 100  152  34  32  120  60   0   84  90   0   40 114   0    8 124   0    0 118  40    0 102 120    0   0   0
		236 238 236   76 154 236  120 124 236  176  98 236  228  84 236  236  88 180  236 106 100  212 136  32  160 170   0  116 196   0   76 208  32   56 204 108   56 180 204   60  60  60
		236 238 236  168 204 236  188 188 236  212 178 236  236 174 236  236 174 212  236 180 176  228 196 144  204 210 120  180 222 120  168 226 144  152 226 180  160 214 228  160 162 160*/
};


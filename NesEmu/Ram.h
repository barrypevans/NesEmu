#pragma once
#include "IMemoryDevice.h"

class Ram : public IMemoryDevice
{
public:
	Ram(uint16_t size);
	~Ram();

	void UploadProgram(uint8_t* bin, uint16_t size, uint16_t location);

	// Inherited via IMemoryDevice
	virtual uint8_t Read(uint16_t addr) override;
	virtual bool Write(uint16_t addr, uint8_t data) override;
	virtual uint16_t GetSize() override;

private:
	uint8_t* m_memory;
	uint16_t m_size;

};


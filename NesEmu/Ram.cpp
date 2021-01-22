#include "Ram.h"
#include <iostream>
#include <stdlib.h>
#include <assert.h>

Ram::Ram(uint16_t size)
{
	m_memory = (uint8_t*)malloc(size);
	m_size = size;
}

Ram::~Ram()
{
	free(m_memory);
}

void Ram::UploadProgram(uint8_t * bin, uint16_t size, uint16_t location)
{
	// set the PC starting register
	//Write(0xFFFC, (location & 0x00FF));
	//Write(0xFFFD, ((location >> 2) & 0x00FF));
	//memcpy(&m_memory[location], bin, size);
}

uint8_t Ram::Read(uint16_t addr)
{
	// Make sure we are not reading outsize of range
	if (addr < m_size)
		return m_memory[addr];
	else
		return -1;
}

bool Ram::Write(uint16_t addr, uint8_t data)
{
	if (addr >= m_size)
		return false;
	m_memory[addr] = data;
	return true;
}

uint16_t Ram::GetSize()
{
	return m_size;
}

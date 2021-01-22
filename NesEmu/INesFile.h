#pragma once
#include <stdint.h>

struct INesHeader
{
	char name[4];
	uint8_t prgChunks;
	uint8_t chrChunks;
	uint8_t mapper1;
	uint8_t mapper2;
	uint8_t prgRamSize;
	uint8_t tvSys1;
	uint8_t tvSys2;
	char unused[5];
};

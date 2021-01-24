#pragma once
#include <stdint.h>
#include <string>
#include <fstream>

class INes
{
public:
	struct INesHeader
	{
		char name[4];
		uint8_t numPrgChunks;
		uint8_t numChrChunks;
		uint8_t mapper1;
		uint8_t mapper2;
		uint8_t prgRamSize;
		uint8_t tvSys1;
		uint8_t tvSys2;
		uint8_t unused[5];
	};

	static void ReadINesFile(std::string filepath, uint8_t*& pPrgMemory, uint8_t*& pChrMemory, INesHeader & header);
};
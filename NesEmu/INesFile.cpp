#include "INesFile.h"
#include <assert.h>

void INes::ReadINesFile(std::string filepath, uint8_t*& pPrgMemory, uint8_t*& pChrMemory, INesHeader & header)
{
	pPrgMemory = nullptr;
	pChrMemory = nullptr;

	// check that file can be read
	std::ifstream file(filepath.c_str(), std::ifstream::binary);
	if (!file.good()) return;

	// read header
	file.read((char*)&header, sizeof(INesHeader));

	// make sure file is not corrupt
	if (!(header.name[0] == 'N' &&
		header.name[1] == 'E' &&
		header.name[2] == 'S' &&
		header.name[3] == 0x1A)) return;

	if ((header.mapper1 & 0x0C) == 0x08)
		assert("NES 2.0 not supported!");

	// skip trainer block
	if(header.mapper1 & 0x04)
		file.seekg(512, std::ios_base::cur);

	// Load prg and chr memory from file.
	const int prgRomSize = header.numPrgChunks * 16384;
	const int chrRamSize = header.numChrChunks * 8192;
	pPrgMemory = (uint8_t*)malloc(prgRomSize);
	pChrMemory = (uint8_t*)malloc(chrRamSize);
	file.read((char*)pPrgMemory, prgRomSize);
	file.read((char*)pChrMemory, chrRamSize);

	file.close();
}

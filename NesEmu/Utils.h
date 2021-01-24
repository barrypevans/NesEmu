#pragma once
#include "cpu6502.h"
#include "bus.h"

#include <QString>
#include <fstream>
#include <vector>

class Utils
{
public:
	struct DisassembledInstruction
	{
		QString mnemonic;
		uint16_t start;
		uint16_t end;
	};

	static QString IntToHexString(int val, int padding = 2)
	{
		return QString::number(val, 16).rightJustified(padding, '0').toUpper();
	}

	static std::vector<DisassembledInstruction> Dissassemble(Bus* pCpuBus, uint16_t startAddr, uint16_t length)
	{
		std::vector<DisassembledInstruction> result;
		uint16_t pc = startAddr;
		while (pc < startAddr+length)
		{
			DisassembledInstruction resultOp;
			resultOp.start = pc;

			uint8_t op = pCpuBus->Read(pc);
			Cpu6502::Instruction inst = Cpu6502::kInstructions[op];
			resultOp.mnemonic = QString("%1: %2 ").arg(IntToHexString(pc, 4), inst.name.c_str());
			pc++;

			if (inst.addrmode == &Cpu6502::IMP)
			{
				resultOp.mnemonic += "<IMP>\n";
			}
			else if (inst.addrmode == &Cpu6502::IMM)
			{
				resultOp.mnemonic += QString("#$%1 <IMM>\n").arg(IntToHexString(pCpuBus->Read(pc++)));
			}
			else if (inst.addrmode == &Cpu6502::ZP0)
			{
				resultOp.mnemonic += QString("$%1 <ZP0>\n").arg(IntToHexString(pCpuBus->Read(pc++)));
			}
			else if (inst.addrmode == &Cpu6502::ZPX)
			{
				resultOp.mnemonic += QString("$%1, X <ZPX>\n").arg(IntToHexString(pCpuBus->Read(pc++)));
			}
			else if (inst.addrmode == &Cpu6502::ZPY)
			{
				resultOp.mnemonic += QString("$%1, Y <ZPY>\n").arg(IntToHexString(pCpuBus->Read(pc++)));
			}
			else if (inst.addrmode == &Cpu6502::IZX)
			{
				resultOp.mnemonic += QString("($%1, X) <IZX>\n").arg(IntToHexString(pCpuBus->Read(pc++)));
			}
			else if (inst.addrmode == &Cpu6502::IZY)
			{
				resultOp.mnemonic += QString("($%1, Y) <IZY>\n").arg(IntToHexString(pCpuBus->Read(pc++)));
			}
			else if (inst.addrmode == &Cpu6502::ABS)
			{
				uint16_t lo = pCpuBus->Read(pc++);
				uint16_t hi = pCpuBus->Read(pc++);

				resultOp.mnemonic += QString("$%1 <ABS>\n").arg(IntToHexString(((hi << 8) | lo), 4));
			}
			else if (inst.addrmode == &Cpu6502::ABX)
			{
				uint16_t lo = pCpuBus->Read(pc++);
				uint16_t hi = pCpuBus->Read(pc++);

				resultOp.mnemonic += QString("$%1, X <ABX>\n").arg(IntToHexString((hi << 8) | lo, 4));
			}
			else if (inst.addrmode == &Cpu6502::ABY)
			{
				uint16_t lo = pCpuBus->Read(pc++);
				uint16_t hi = pCpuBus->Read(pc++);

				resultOp.mnemonic += QString("$%1, Y <ABY>\n").arg(IntToHexString((hi << 8) | lo, 4));
			}
			else if (inst.addrmode == &Cpu6502::IND)
			{
				uint16_t lo = pCpuBus->Read(pc++);
				uint16_t hi = pCpuBus->Read(pc++);

				resultOp.mnemonic += QString("$%1 <IND>\n").arg(IntToHexString((hi << 8) | lo, 4));
			}
			else if (inst.addrmode == &Cpu6502::REL)
			{
				uint16_t offset = pCpuBus->Read(pc++);
				if (offset & 0x0080 > 0)
					offset |= 0xFF00;
				uint16_t absoluteAddr = pc + offset;
				resultOp.mnemonic += QString("$%1 [%2] <REL>\n").arg(IntToHexString(offset), IntToHexString(absoluteAddr, 4));
			}
			resultOp.end = pc - 1;
			result.push_back(resultOp);
		}

		return result;
	}

	static void* Utils::LoadEntireFile(const char * filePath, size_t& length, bool nullTerminate)
	{
		std::ifstream ifs(filePath, std::ios::binary | std::ios::ate);

		if (!ifs.good())
			return nullptr;

		length = ifs.tellg();

		char* data = nullptr;
		if (nullTerminate)
			data = (char*)malloc(length + 1);
		else
			data = (char*)malloc(length);

		ifs.seekg(0, std::ios::beg);
		ifs.read(data, length);

		if (nullTerminate)
		{
			data[length] = '\0';
			length++;
		}

		return data;
	}

	static void Utils::DumpToFile(const char * filePath, void* pData, size_t size)
	{
		std::ofstream outStream; // outdata is like cin
		outStream.open(filePath, std::ios::binary | std::ios::ate); // opens the file
		outStream.write((char*)pData, size);
		outStream.close();
	}
};
#include "cpu6502.h"

Cpu6502::Instruction Cpu6502::kInstructions[256] =
{
{ "BRK", &Cpu6502::BRK, &Cpu6502::IMM, 7 },{ "ORA", &Cpu6502::ORA, &Cpu6502::IZX, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 8 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 3 },{ "ORA", &Cpu6502::ORA, &Cpu6502::ZP0, 3 },{ "ASL", &Cpu6502::ASL, &Cpu6502::ZP0, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 5 },{ "PHP", &Cpu6502::PHP, &Cpu6502::IMP, 3 },{ "ORA", &Cpu6502::ORA, &Cpu6502::IMM, 2 },{ "ASL", &Cpu6502::ASL, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 4 },{ "ORA", &Cpu6502::ORA, &Cpu6502::ABS, 4 },{ "ASL", &Cpu6502::ASL, &Cpu6502::ABS, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },
{ "BPL", &Cpu6502::BPL, &Cpu6502::REL, 2 },{ "ORA", &Cpu6502::ORA, &Cpu6502::IZY, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 8 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 4 },{ "ORA", &Cpu6502::ORA, &Cpu6502::ZPX, 4 },{ "ASL", &Cpu6502::ASL, &Cpu6502::ZPX, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },{ "CLC", &Cpu6502::CLC, &Cpu6502::IMP, 2 },{ "ORA", &Cpu6502::ORA, &Cpu6502::ABY, 4 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 7 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 4 },{ "ORA", &Cpu6502::ORA, &Cpu6502::ABX, 4 },{ "ASL", &Cpu6502::ASL, &Cpu6502::ABX, 7 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 7 },
{ "JSR", &Cpu6502::JSR, &Cpu6502::ABS, 6 },{ "AND", &Cpu6502::AND, &Cpu6502::IZX, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 8 },{ "BIT", &Cpu6502::BIT, &Cpu6502::ZP0, 3 },{ "AND", &Cpu6502::AND, &Cpu6502::ZP0, 3 },{ "ROL", &Cpu6502::ROL, &Cpu6502::ZP0, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 5 },{ "PLP", &Cpu6502::PLP, &Cpu6502::IMP, 4 },{ "AND", &Cpu6502::AND, &Cpu6502::IMM, 2 },{ "ROL", &Cpu6502::ROL, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "BIT", &Cpu6502::BIT, &Cpu6502::ABS, 4 },{ "AND", &Cpu6502::AND, &Cpu6502::ABS, 4 },{ "ROL", &Cpu6502::ROL, &Cpu6502::ABS, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },
{ "BMI", &Cpu6502::BMI, &Cpu6502::REL, 2 },{ "AND", &Cpu6502::AND, &Cpu6502::IZY, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 8 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 4 },{ "AND", &Cpu6502::AND, &Cpu6502::ZPX, 4 },{ "ROL", &Cpu6502::ROL, &Cpu6502::ZPX, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },{ "SEC", &Cpu6502::SEC, &Cpu6502::IMP, 2 },{ "AND", &Cpu6502::AND, &Cpu6502::ABY, 4 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 7 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 4 },{ "AND", &Cpu6502::AND, &Cpu6502::ABX, 4 },{ "ROL", &Cpu6502::ROL, &Cpu6502::ABX, 7 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 7 },
{ "RTI", &Cpu6502::RTI, &Cpu6502::IMP, 6 },{ "EOR", &Cpu6502::EOR, &Cpu6502::IZX, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 8 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 3 },{ "EOR", &Cpu6502::EOR, &Cpu6502::ZP0, 3 },{ "LSR", &Cpu6502::LSR, &Cpu6502::ZP0, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 5 },{ "PHA", &Cpu6502::PHA, &Cpu6502::IMP, 3 },{ "EOR", &Cpu6502::EOR, &Cpu6502::IMM, 2 },{ "LSR", &Cpu6502::LSR, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "JMP", &Cpu6502::JMP, &Cpu6502::ABS, 3 },{ "EOR", &Cpu6502::EOR, &Cpu6502::ABS, 4 },{ "LSR", &Cpu6502::LSR, &Cpu6502::ABS, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },
{ "BVC", &Cpu6502::BVC, &Cpu6502::REL, 2 },{ "EOR", &Cpu6502::EOR, &Cpu6502::IZY, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 8 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 4 },{ "EOR", &Cpu6502::EOR, &Cpu6502::ZPX, 4 },{ "LSR", &Cpu6502::LSR, &Cpu6502::ZPX, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },{ "CLI", &Cpu6502::CLI, &Cpu6502::IMP, 2 },{ "EOR", &Cpu6502::EOR, &Cpu6502::ABY, 4 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 7 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 4 },{ "EOR", &Cpu6502::EOR, &Cpu6502::ABX, 4 },{ "LSR", &Cpu6502::LSR, &Cpu6502::ABX, 7 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 7 },
{ "RTS", &Cpu6502::RTS, &Cpu6502::IMP, 6 },{ "ADC", &Cpu6502::ADC, &Cpu6502::IZX, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 8 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 3 },{ "ADC", &Cpu6502::ADC, &Cpu6502::ZP0, 3 },{ "ROR", &Cpu6502::ROR, &Cpu6502::ZP0, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 5 },{ "PLA", &Cpu6502::PLA, &Cpu6502::IMP, 4 },{ "ADC", &Cpu6502::ADC, &Cpu6502::IMM, 2 },{ "ROR", &Cpu6502::ROR, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "JMP", &Cpu6502::JMP, &Cpu6502::IND, 5 },{ "ADC", &Cpu6502::ADC, &Cpu6502::ABS, 4 },{ "ROR", &Cpu6502::ROR, &Cpu6502::ABS, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },
{ "BVS", &Cpu6502::BVS, &Cpu6502::REL, 2 },{ "ADC", &Cpu6502::ADC, &Cpu6502::IZY, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 8 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 4 },{ "ADC", &Cpu6502::ADC, &Cpu6502::ZPX, 4 },{ "ROR", &Cpu6502::ROR, &Cpu6502::ZPX, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },{ "SEI", &Cpu6502::SEI, &Cpu6502::IMP, 2 },{ "ADC", &Cpu6502::ADC, &Cpu6502::ABY, 4 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 7 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 4 },{ "ADC", &Cpu6502::ADC, &Cpu6502::ABX, 4 },{ "ROR", &Cpu6502::ROR, &Cpu6502::ABX, 7 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 7 },
{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 2 },{ "STA", &Cpu6502::STA, &Cpu6502::IZX, 6 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },{ "STY", &Cpu6502::STY, &Cpu6502::ZP0, 3 },{ "STA", &Cpu6502::STA, &Cpu6502::ZP0, 3 },{ "STX", &Cpu6502::STX, &Cpu6502::ZP0, 3 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 3 },{ "DEY", &Cpu6502::DEY, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 2 },{ "TXA", &Cpu6502::TXA, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "STY", &Cpu6502::STY, &Cpu6502::ABS, 4 },{ "STA", &Cpu6502::STA, &Cpu6502::ABS, 4 },{ "STX", &Cpu6502::STX, &Cpu6502::ABS, 4 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 4 },
{ "BCC", &Cpu6502::BCC, &Cpu6502::REL, 2 },{ "STA", &Cpu6502::STA, &Cpu6502::IZY, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },{ "STY", &Cpu6502::STY, &Cpu6502::ZPX, 4 },{ "STA", &Cpu6502::STA, &Cpu6502::ZPX, 4 },{ "STX", &Cpu6502::STX, &Cpu6502::ZPY, 4 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 4 },{ "TYA", &Cpu6502::TYA, &Cpu6502::IMP, 2 },{ "STA", &Cpu6502::STA, &Cpu6502::ABY, 5 },{ "TXS", &Cpu6502::TXS, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 5 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 5 },{ "STA", &Cpu6502::STA, &Cpu6502::ABX, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 5 },
{ "LDY", &Cpu6502::LDY, &Cpu6502::IMM, 2 },{ "LDA", &Cpu6502::LDA, &Cpu6502::IZX, 6 },{ "LDX", &Cpu6502::LDX, &Cpu6502::IMM, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },{ "LDY", &Cpu6502::LDY, &Cpu6502::ZP0, 3 },{ "LDA", &Cpu6502::LDA, &Cpu6502::ZP0, 3 },{ "LDX", &Cpu6502::LDX, &Cpu6502::ZP0, 3 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 3 },{ "TAY", &Cpu6502::TAY, &Cpu6502::IMP, 2 },{ "LDA", &Cpu6502::LDA, &Cpu6502::IMM, 2 },{ "TAX", &Cpu6502::TAX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "LDY", &Cpu6502::LDY, &Cpu6502::ABS, 4 },{ "LDA", &Cpu6502::LDA, &Cpu6502::ABS, 4 },{ "LDX", &Cpu6502::LDX, &Cpu6502::ABS, 4 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 4 },
{ "BCS", &Cpu6502::BCS, &Cpu6502::REL, 2 },{ "LDA", &Cpu6502::LDA, &Cpu6502::IZY, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 5 },{ "LDY", &Cpu6502::LDY, &Cpu6502::ZPX, 4 },{ "LDA", &Cpu6502::LDA, &Cpu6502::ZPX, 4 },{ "LDX", &Cpu6502::LDX, &Cpu6502::ZPY, 4 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 4 },{ "CLV", &Cpu6502::CLV, &Cpu6502::IMP, 2 },{ "LDA", &Cpu6502::LDA, &Cpu6502::ABY, 4 },{ "TSX", &Cpu6502::TSX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 4 },{ "LDY", &Cpu6502::LDY, &Cpu6502::ABX, 4 },{ "LDA", &Cpu6502::LDA, &Cpu6502::ABX, 4 },{ "LDX", &Cpu6502::LDX, &Cpu6502::ABY, 4 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 4 },
{ "CPY", &Cpu6502::CPY, &Cpu6502::IMM, 2 },{ "CMP", &Cpu6502::CMP, &Cpu6502::IZX, 6 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 8 },{ "CPY", &Cpu6502::CPY, &Cpu6502::ZP0, 3 },{ "CMP", &Cpu6502::CMP, &Cpu6502::ZP0, 3 },{ "DEC", &Cpu6502::DEC, &Cpu6502::ZP0, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 5 },{ "INY", &Cpu6502::INY, &Cpu6502::IMP, 2 },{ "CMP", &Cpu6502::CMP, &Cpu6502::IMM, 2 },{ "DEX", &Cpu6502::DEX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "CPY", &Cpu6502::CPY, &Cpu6502::ABS, 4 },{ "CMP", &Cpu6502::CMP, &Cpu6502::ABS, 4 },{ "DEC", &Cpu6502::DEC, &Cpu6502::ABS, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },
{ "BNE", &Cpu6502::BNE, &Cpu6502::REL, 2 },{ "CMP", &Cpu6502::CMP, &Cpu6502::IZY, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 8 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 4 },{ "CMP", &Cpu6502::CMP, &Cpu6502::ZPX, 4 },{ "DEC", &Cpu6502::DEC, &Cpu6502::ZPX, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },{ "CLD", &Cpu6502::CLD, &Cpu6502::IMP, 2 },{ "CMP", &Cpu6502::CMP, &Cpu6502::ABY, 4 },{ "NOP", &Cpu6502::NOP, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 7 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 4 },{ "CMP", &Cpu6502::CMP, &Cpu6502::ABX, 4 },{ "DEC", &Cpu6502::DEC, &Cpu6502::ABX, 7 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 7 },
{ "CPX", &Cpu6502::CPX, &Cpu6502::IMM, 2 },{ "SBC", &Cpu6502::SBC, &Cpu6502::IZX, 6 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 8 },{ "CPX", &Cpu6502::CPX, &Cpu6502::ZP0, 3 },{ "SBC", &Cpu6502::SBC, &Cpu6502::ZP0, 3 },{ "INC", &Cpu6502::INC, &Cpu6502::ZP0, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 5 },{ "INX", &Cpu6502::INX, &Cpu6502::IMP, 2 },{ "SBC", &Cpu6502::SBC, &Cpu6502::IMM, 2 },{ "NOP", &Cpu6502::NOP, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::SBC, &Cpu6502::IMP, 2 },{ "CPX", &Cpu6502::CPX, &Cpu6502::ABS, 4 },{ "SBC", &Cpu6502::SBC, &Cpu6502::ABS, 4 },{ "INC", &Cpu6502::INC, &Cpu6502::ABS, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },
{ "BEQ", &Cpu6502::BEQ, &Cpu6502::REL, 2 },{ "SBC", &Cpu6502::SBC, &Cpu6502::IZY, 5 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 8 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 4 },{ "SBC", &Cpu6502::SBC, &Cpu6502::ZPX, 4 },{ "INC", &Cpu6502::INC, &Cpu6502::ZPX, 6 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 6 },{ "SED", &Cpu6502::SED, &Cpu6502::IMP, 2 },{ "SBC", &Cpu6502::SBC, &Cpu6502::ABY, 4 },{ "NOP", &Cpu6502::NOP, &Cpu6502::IMP, 2 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 7 },{ "???", &Cpu6502::NOP, &Cpu6502::IMP, 4 },{ "SBC", &Cpu6502::SBC, &Cpu6502::ABX, 4 },{ "INC", &Cpu6502::INC, &Cpu6502::ABX, 7 },{ "???", &Cpu6502::XXX, &Cpu6502::IMP, 7 },
};

void Cpu6502::Irq()
{
	if (GetFlag(I))
	{
		m_pCpuBus->Write(0x0100 + sp, (pc >> 8) & 0x00FF);
		sp--;
		m_pCpuBus->Write(0x0100 + sp, pc & 0x00FF);
		sp--;

		SetFlag(B, false);
		SetFlag(U, true);
		SetFlag(I, true);
		m_pCpuBus->Write(0x0100 + sp, status);
		sp--;

		absoluteAddr = 0XFFFE;
		pc = m_pCpuBus->Read16(absoluteAddr);

		remainingCycles = 7;
	}
}

void Cpu6502::Nmi()
{
	m_pCpuBus->Write(0x0100 + sp, (pc >> 8) & 0x00FF);
	sp--;
	m_pCpuBus->Write(0x0100 + sp, pc & 0x00FF);
	sp--;

	SetFlag(B, false);
	SetFlag(U, true);
	SetFlag(I, true);
	m_pCpuBus->Write(0x0100 + sp, status);
	sp--;

	absoluteAddr = 0XFFFA;
	pc = m_pCpuBus->Read16(absoluteAddr);

	remainingCycles = 8;
}


// Addressing modes

/*
	This mode is essentially a NOP.
	The address is implied by the instruction.
*/
uint8_t Cpu6502::IMP()
{
	return 0;
}

/*
	Byte after the opcode is the data.
*/
uint8_t Cpu6502::IMM()
{
	absoluteAddr = pc++;
	return 0;
}

/*
	Data in located at the zero page.
*/
uint8_t Cpu6502::ZP0()
{
	uint16_t zPage = m_pCpuBus->Read(pc++);
	absoluteAddr = 0x00FF & zPage;
	return 0;
}

/*
	Data in located at the zero page, offset by X.
*/
uint8_t Cpu6502::ZPX()
{
	uint16_t zPage = m_pCpuBus->Read(pc++);
	absoluteAddr = 0x00FF & (zPage + X);
	return 0;
}

/*
	Data in located at the zero page, offset by Y.
*/
uint8_t Cpu6502::ZPY()
{
	uint16_t zPage = m_pCpuBus->Read(pc++);
	absoluteAddr = 0x00FF & (zPage + Y);
	return 0;
}

/*
	Data is located at a address that is relative to the program counter (signed)
*/
uint8_t Cpu6502::REL()
{
	relativeAddr = m_pCpuBus->Read(pc++);

	// if the MSB is set, we know its negative.
	if (relativeAddr & 0x80)
		relativeAddr |= 0xFF00;

	return 0;
}

/*
	Next two bytes point to the data
*/
uint8_t Cpu6502::ABS()
{
	absoluteAddr = m_pCpuBus->Read16(pc);
	pc += 2;
	return 0;
}

/*
	Next two bytes plus the X register point to the data
*/
uint8_t Cpu6502::ABX()
{
	uint16_t addrPreOffset = m_pCpuBus->Read16(pc);
	pc += 2;
	absoluteAddr = addrPreOffset + (uint16_t)X;

	// check for page boundry cross
	return (addrPreOffset & 0xFF00) == (absoluteAddr & 0xFF00) ? 0 : 1;
}

/*
	Next two bytes plus the Y register point to the data
*/
uint8_t Cpu6502::ABY()
{
	uint16_t addrPreOffset = m_pCpuBus->Read16(pc);
	pc += 2;
	absoluteAddr = addrPreOffset + (uint16_t)Y;

	// check for page boundry cross
	return (addrPreOffset & 0xFF00) == (absoluteAddr & 0xFF00) ? 0 : 1;
}

/*

*/
uint8_t Cpu6502::IND()
{
	uint16_t indirectAddr = m_pCpuBus->Read16(pc);
	pc += 2;

	if (indirectAddr == (indirectAddr & 0x00FF)) // Simulate page boundary hardware bug
	{
		absoluteAddr = (m_pCpuBus->Read(indirectAddr & 0xFF00) << 8) | m_pCpuBus->Read(indirectAddr);
	}
	else // Behave normally
	{
		absoluteAddr = m_pCpuBus->Read16(indirectAddr);
	}
	return 0;
}

uint8_t Cpu6502::IZX()
{
	uint16_t indirectAddr = m_pCpuBus->Read(pc++);
	absoluteAddr = m_pCpuBus->Read16(indirectAddr + (uint16_t)X);
	return 0;
}

uint8_t Cpu6502::IZY()
{
	uint16_t indirectAddr = m_pCpuBus->Read(pc++);
	uint16_t addrPreOffset = m_pCpuBus->Read16(indirectAddr);
	absoluteAddr = addrPreOffset + (uint16_t)Y;

	// check for page boundry cross
	return (addrPreOffset & 0xFF00) == (absoluteAddr & 0xFF00) ? 0 : 1;
}

// Opcodes

uint8_t Cpu6502::ADC()
{
	uint16_t data = (uint16_t)FetchData();
	uint16_t result = (uint16_t)A + (uint16_t)GetFlag(C) + data;

	SetFlag(C, result > 0x00FF);
	SetFlag(Z, (result & 0x00FF) == 0);
	// This comes from a long as mother fuckin truth table....
	SetFlag(V, (~((uint16_t)A ^ data) & ((uint16_t)A ^ result)) & 0x0080);
	SetFlag(N, result & 0x0080);
	A = result;

	return 1;
}

uint8_t Cpu6502::AND()
{
	A &= (uint8_t)FetchData();
	SetFlag(N, A & 0x80);
	SetFlag(C, A == 0x00);
	return 1;
}

uint8_t Cpu6502::ASL()
{
	uint16_t temp = FetchData();
	temp = temp << 1;
	SetFlag(N, temp & 0x0080);
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(C, temp > 0x00FF);

	if (kInstructions[currentOpCode].addrmode == &Cpu6502::IMP)
		A = temp & 0x00FF;
	else
		m_pCpuBus->Write(absoluteAddr, temp & 0x00FF);

	return 0;
}

uint8_t Cpu6502::BCC()
{
	if (!GetFlag(C))
	{
		remainingCycles++;
		absoluteAddr = pc + relativeAddr;

		// check for page boundry
		if ((absoluteAddr & 0xFF00) != (pc & 0xFF00))
			remainingCycles++;

		pc = absoluteAddr;
	}
	return 0;
}

uint8_t Cpu6502::BCS()
{
	if (GetFlag(C))
	{
		remainingCycles++;
		absoluteAddr = pc + relativeAddr;

		// check for page boundry
		if ((absoluteAddr & 0xFF00) != (pc & 0xFF00))
			remainingCycles++;

		pc = absoluteAddr;
	}
	return 0;
}

uint8_t Cpu6502::BEQ()
{
	if (GetFlag(Z))
	{
		remainingCycles++;
		absoluteAddr = pc + relativeAddr;

		// check for page boundry
		if ((absoluteAddr & 0xFF00) != (pc & 0xFF00))
			remainingCycles++;

		pc = absoluteAddr;
	}
	return 0;
}

uint8_t Cpu6502::BIT()
{
	uint8_t temp = FetchData();
	SetFlag(Z, (A & temp) == 0);
	SetFlag(N, (temp & 0x40));
	SetFlag(V, (temp & 0x60));
	return 0;
}

uint8_t Cpu6502::BMI()
{
	if (GetFlag(N))
	{
		remainingCycles++;
		absoluteAddr = pc + relativeAddr;

		// check for page boundry
		if ((absoluteAddr & 0xFF00) != (pc & 0xFF00))
			remainingCycles++;

		pc = absoluteAddr;
	}
	return 0;
}

uint8_t Cpu6502::BNE()
{
	if (!GetFlag(Z))
	{
		remainingCycles++;
		absoluteAddr = pc + relativeAddr;

		// check for page boundry
		if ((absoluteAddr & 0xFF00) != (pc & 0xFF00))
			remainingCycles++;

		pc = absoluteAddr;
	}
	return 0;
}

uint8_t Cpu6502::BPL()
{
	if (!GetFlag(N))
	{
		remainingCycles++;
		absoluteAddr = pc + relativeAddr;

		// check for page boundry
		if ((absoluteAddr & 0xFF00) != (pc & 0xFF00))
			remainingCycles++;

		pc = absoluteAddr;
	}
	return 0;
}

//TODO: Read the manual on this shit
uint8_t Cpu6502::BRK()
{
	SetFlag(I, 1);
	m_pCpuBus->Write(0x0100 + (uint16_t)sp, (pc >> 8) & 0x00FF);
	sp--;
	m_pCpuBus->Write(0x0100 + (uint16_t)sp, pc & 0x00FF);
	sp--;

	SetFlag(B, 1);
	m_pCpuBus->Write(0x0100 + (uint16_t)sp, status);
	sp--;
	SetFlag(B, 0);

	pc = (uint16_t)m_pCpuBus->Read(0xFFFE) | ((uint16_t)m_pCpuBus->Read(0xFFFF) << 8);
	return 0;
}

uint8_t Cpu6502::BVC()
{
	if (!GetFlag(V))
	{
		remainingCycles++;
		absoluteAddr = pc + relativeAddr;

		// check for page boundry
		if ((absoluteAddr & 0xFF00) != (pc & 0xFF00))
			remainingCycles++;

		pc = absoluteAddr;
	}
	return 0;
}

uint8_t Cpu6502::BVS()
{
	if (GetFlag(V))
	{
		remainingCycles++;
		absoluteAddr = pc + relativeAddr;

		// check for page boundry
		if ((absoluteAddr & 0xFF00) != (pc & 0xFF00))
			remainingCycles++;

		pc = absoluteAddr;
	}
	return 0;
}

uint8_t Cpu6502::CLC()
{
	SetFlag(C, 0);
	return 0;
}

uint8_t Cpu6502::CLD()
{
	SetFlag(D, 0);
	return 0;
}

uint8_t Cpu6502::CLI()
{
	SetFlag(I, 0);
	return 0;
}

uint8_t Cpu6502::CLV()
{
	SetFlag(V, 0);
	return 0;
}

uint8_t Cpu6502::CMP()
{
	uint16_t data = FetchData();
	uint16_t temp = (uint16_t)A - data;
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(N, temp & 0x0080);
	SetFlag(C, A >= data);
	return 0;
}

uint8_t Cpu6502::CPX()
{
	uint16_t data = FetchData();
	uint16_t temp = (uint16_t)X - data;
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(N, temp & 0x0080);
	SetFlag(C, X >= data);
	return 0;
}

uint8_t Cpu6502::CPY()
{
	uint16_t data = FetchData();
	uint16_t temp = (uint16_t)Y - data;
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(N, temp & 0x0080);
	SetFlag(C, Y >= data);
	return 0;
}

uint8_t Cpu6502::DEC()
{
	uint8_t data = FetchData() - 1;
	m_pCpuBus->Write(absoluteAddr, data);
	SetFlag(Z, data == 0);
	SetFlag(N, data & 0x80);
	return 0;
}

uint8_t Cpu6502::DEX()
{
	X--;
	SetFlag(Z, X == 0);
	SetFlag(N, X & 0x80);
	return 0;
}

uint8_t Cpu6502::DEY()
{
	Y--;
	SetFlag(Z, Y == 0);
	SetFlag(N, Y & 0x80);
	return 0;;
}

uint8_t Cpu6502::EOR()
{
	uint8_t data = FetchData();
	uint8_t temp = A ^ data;
	SetFlag(Z, temp == 0x00);
	SetFlag(N, temp & 0x80);
	A = temp;
	return 1;
}

uint8_t Cpu6502::INC()
{
	uint8_t data = FetchData() + 1;
	m_pCpuBus->Write(absoluteAddr, data);
	SetFlag(Z, data == 0);
	SetFlag(N, data & 0x80);
	return 0;
}

uint8_t Cpu6502::INX()
{
	X++;
	SetFlag(Z, X == 0);
	SetFlag(N, X & 0x80);
	return 0;
}

uint8_t Cpu6502::INY()
{
	Y++;
	SetFlag(Z, Y == 0);
	SetFlag(N, Y & 0x80);
	return 0;
}

uint8_t Cpu6502::JMP()
{
	pc = absoluteAddr;
	return 0;
}

uint8_t Cpu6502::JSR()
{
	pc--;

	m_pCpuBus->Write(0x0100 + (uint16_t)sp, (pc >> 8) & 0x00FF);
	sp--;
	m_pCpuBus->Write(0x0100 + (uint16_t)sp, pc & 0x00FF);
	sp--;

	pc = absoluteAddr;
	return 0;
}

uint8_t Cpu6502::LDA()
{
	A = FetchData();
	SetFlag(Z, A == 0);
	SetFlag(N, A & 0x80);
	return 1;
}

uint8_t Cpu6502::LDX()
{
	X = FetchData();
	SetFlag(Z, X == 0);
	SetFlag(N, X & 0x80);
	return 1;
}

uint8_t Cpu6502::LDY()
{
	Y = FetchData();
	SetFlag(Z, Y == 0);
	SetFlag(N, Y & 0x80);
	return 1;
}

uint8_t Cpu6502::LSR()
{
	uint16_t temp = FetchData();

	SetFlag(C, temp & 0x0001);
	temp = temp >> 1;

	SetFlag(N, false);
	SetFlag(Z, (temp & 0x00FF) == 0);

	if (kInstructions[currentOpCode].addrmode == &Cpu6502::IMP)
		A = temp & 0x00FF;
	else
		m_pCpuBus->Write(absoluteAddr, temp & 0x00FF);

	return 0;
}

uint8_t Cpu6502::NOP()
{
	// apparently some NOPs can take an extra cycle
	// why? Idfk, read the manual.
	switch (currentOpCode)
	{
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}

uint8_t Cpu6502::ORA()
{
	A = A | FetchData();
	SetFlag(N, A & 0x80);
	SetFlag(Z, A == 0x00);
	return 1;
}

uint8_t Cpu6502::PHA()
{
	m_pCpuBus->Write(0x0100 + (uint16_t)sp, A);
	sp--;
	return 0;
}

uint8_t Cpu6502::PHP()
{
	m_pCpuBus->Write(0x0100 + (uint16_t)sp, status);
	sp--;
	return 0;
}

uint8_t Cpu6502::PLA()
{
	sp++;
	A = m_pCpuBus->Read(0x0100 + (uint16_t)sp);
	SetFlag(N, A & 0x80);
	SetFlag(Z, A == 0x00);
	return 0;
}

uint8_t Cpu6502::PLP()
{
	sp++;
	status = m_pCpuBus->Read(0x0100 + (uint16_t)sp);
	return 0;
}

uint8_t Cpu6502::ROL()
{
	uint16_t temp = (FetchData() << 1) | GetFlag(C);
	SetFlag(N, (temp & 0x00FF) & 0x0080);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(C, temp & 0xFF00);
	return 0;
}

uint8_t Cpu6502::ROR()
{
	uint16_t temp = ((uint16_t)GetFlag(C) << 7) | (FetchData() >> 1);
	SetFlag(N, (temp & 0x00FF) & 0x0080);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(C, temp & 0xFF00);
	return 0;
}

uint8_t Cpu6502::RTI()
{
	sp++;
	status = m_pCpuBus->Read(0x0100 + (uint16_t)sp);
	SetFlag(B, 0);
	SetFlag(U, 0);

	sp++;
	pc = m_pCpuBus->Read16(0x0100 + (uint16_t)sp);
	sp++;

	return 0;
}

uint8_t Cpu6502::RTS()
{
	sp++;
	pc = m_pCpuBus->Read16(0x0100 + (uint16_t)sp);
	sp++;
	pc++;
	return 0;
}

uint8_t Cpu6502::SBC()
{
	uint16_t data = ~(uint16_t)FetchData() + 0x0001;
	uint16_t result = (uint16_t)A + (uint16_t)GetFlag(C) + data;

	SetFlag(C, result > 0x00FF);
	SetFlag(Z, (result & 0x00FF) == 0);
	// This comes from a long as mother fuckin truth table....
	SetFlag(V, (~(A ^ data) & (A ^ result)) & 0x0080);
	SetFlag(N, result & 0x0080);
	A = result;

	return 1;
}

uint8_t Cpu6502::SEC()
{
	SetFlag(C, 1);
	return 0;
}

uint8_t Cpu6502::SED()
{
	SetFlag(D, 1);
	return 0;
}

uint8_t Cpu6502::SEI()
{
	SetFlag(I, 1);
	return 0;
}

uint8_t Cpu6502::STA()
{
	m_pCpuBus->Write(absoluteAddr, A);
	return 0;
}

uint8_t Cpu6502::STX()
{
	m_pCpuBus->Write(absoluteAddr, X);
	return 0;
}

uint8_t Cpu6502::STY()
{
	m_pCpuBus->Write(absoluteAddr, Y);
	return 0;
}

uint8_t Cpu6502::TAX()
{
	X = A;
	SetFlag(N, X & 0x80);
	SetFlag(Z, X == 0x00);
	return 0;
}

uint8_t Cpu6502::TAY()
{
	Y = A;
	SetFlag(N, Y & 0x80);
	SetFlag(Z, Y == 0x00);
	return 0;
}

uint8_t Cpu6502::TSX()
{
	X = sp;
	SetFlag(N, X & 0x80);
	SetFlag(Z, X == 0x00);
	return 0;
}

uint8_t Cpu6502::TXA()
{
	A = X;
	SetFlag(N, A & 0x80);
	SetFlag(Z, A == 0x00);
	return 0;

}

uint8_t Cpu6502::TXS()
{
	sp = X;
	SetFlag(N, sp & 0x80);
	SetFlag(Z, sp == 0x00);
	return 0;
}

uint8_t Cpu6502::TYA()
{
	A = Y;
	SetFlag(N, A & 0x80);
	SetFlag(Z, A == 0x00);
	return 0;
}

uint8_t Cpu6502::XXX()
{
	return 0;
}

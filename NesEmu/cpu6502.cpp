#include "cpu6502.h"
#include <stdlib.h>
#include "Utils.h"
#include <QDebug>

Cpu6502::Cpu6502(Bus* pCpuBus, Bus* pPpuBus)
{
	m_pCpuBus = pCpuBus;
	m_pPpuBus = pPpuBus;
	Reset();
}

Cpu6502::~Cpu6502()
{
}

/*
	Reset cpu state and internal helper state.
	This operation takes 8 cycles.
*/
void Cpu6502::Reset()
{
	// reset registers
	A = 0x00;
	X = 0x00;
	Y = 0x00;
	status = 0x00 | U | I;

	// stack starts at 0x01FD
	sp = 0xFD;

	// initial program counter adderess is always storead at 0xFFFC
	const uint16_t pcInitAddr = 0xFFFC;
	uint16_t lo = m_pCpuBus->Read(pcInitAddr);
	uint16_t hi = m_pCpuBus->Read(pcInitAddr+1);
	pc = (hi << 8) + lo;
	pc = 0xc000;
	
	// reset helper state
	currentOpCode = 0x00;
	absoluteAddr = 0x0000;
	relativeAddr = 0x0000;

	// reset takes 8 cycles
	remainingCycles = 0x07;
	m_cycles = 0;
}

/*
	Get data that was previously fetched by an addressing mode function
*/
uint8_t Cpu6502::FetchData()
{
	if (kInstructions[currentOpCode].addrmode == &Cpu6502::IMP)
		return A;

	return m_pCpuBus->Read(absoluteAddr);
}

/*
	Set the value of a status register flag
*/
void Cpu6502::SetFlag(Flags flag, bool state)
{
	if (state)
	{
		status |= flag;
	}
	else
	{
		status &= ~flag;
	}
}

/*
	Get the value of a status register flag
*/
uint8_t Cpu6502::GetFlag(Flags flag)
{
	return (status & flag) != 0;
}

/*
	Progress the cpu by one cycle
*/
void Cpu6502::Tick()
{
	if (remainingCycles == 0)
	{
		//LogState();
		currentOpCode = m_pCpuBus->Read(pc++);

		Instruction instruction = kInstructions[currentOpCode];

		// Set number of cylces for this instruction
		remainingCycles = instruction.cycles;

		// Unused flag should always be true
		SetFlag(U, true);

		uint8_t addCycleAddr = (this->*instruction.addrmode) ();
		uint8_t addCycleOp = (this->*instruction.operation)();

		// If both the opcode and addressing mode warrent an extra cycle, add one cycle
		remainingCycles += (addCycleOp && addCycleAddr);

		// Set unused flag true again after operation in case it was changed.
		SetFlag(U, true);

		m_instructionComplete = false;
	}
	remainingCycles--;

	if (remainingCycles == 0)
		m_instructionComplete = true;

	m_cycles++;
}

void Cpu6502::LogState()
{
	
	std::vector<Utils::DisassembledInstruction> disasm;
	Utils::DissassembleFix(disasm, m_pCpuBus, pc, 1, false);
	qInfo("%s            A:%x X:%x Y:%x P:%x SP:%x CYC:%d", disasm[0].mnemonic.toStdString().c_str(), A, X, Y, status, sp, m_cycles );
}

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using Byte = unsigned char;		//Memory size of Byte	
using Word = unsigned short;	//Memory size of Word

using u32 = unsigned int;

struct Mem
{
	static constexpr u32 MAX_MEM = 1024 * 64;
	Byte Data[MAX_MEM];

	void Initialise()
	{
		for (u32 i = 0; i < MAX_MEM; i++)
		{
			Data[i] = 0;
		}
	}

	Byte operator[](u32 Address) const
	{
		return Data[Address];
	}

	Byte& operator[](u32 Address)
	{
		return Data[Address];
	}

	void WriteWord(Word Vaule, u32 Address, u32& Cycles)
	{
		Data[Address] = Vaule & 0xFF;
		Data[Address + 1] = (Vaule >> 8);
		Cycles -= 2;
	}
};

struct CPU
{
	Word PC;		//Program Counter
	Word SP;		//Stack Pointer

	Byte X, Y, Z;	//Register

	Byte C : 1;		//Carry flag
	Byte Z : 1;		//Zero flag
	Byte N : 1;		//Negative flag
	Byte V : 1;		//Overflow flag
	Byte S : 1;		//Sign flag
	Byte H : 1;		//Half carry
	Byte T : 1;		//Bit copy storage
	Byte I : 1;		//Global interrupt enable

	void Reset(Mem& memory)
	{
		PC = 0xFFFC;
		SP = 0x1000;

		C = Z = N = V = S = H = T = I = 0;
		Z = X = Y = 0;
		memory.Initialise();
	}

	Byte FetchByte(u32& Cycles, Mem& memory)
	{
		Byte Data = memory[PC];
		PC++;
		Cycles--;
		return Data;
	}

	Word FetchWord(u32& Cycles, Mem& memory)
	{
		Word Data = memory[PC];
		PC++;
		return Data;

		Data |= (memory[PC] << 8);
		PC++;
		Cycles -= 2;
		return Data;

	}

	Byte ReadByte(u32& Cycles, Byte Address, Mem& memory)
	{
		Byte Data = memory[Address];
		Cycles--;
		return Data;

	}

	static constexpr Byte
		//INS_LDA_IM = 0XA9,
		//INS_LDA_ZP = 0XA5,
		//INS_LDA_ZPX = 0xB5,
		//INS_JSR = 0x20;

	/*void LDASetstatus()
	{
		Z = (A == 0);
		N = (A & 0b10000000) > 0;
	}*/


	void Execute(u32 Cycles, Mem& memory)
	{
		while (Cycles > 0)
		{
			/*
			Byte Ins = FetchByte(Cycles, memory);
			switch (Ins)
			{
			case INS_LDA_IM:
			{

				Byte Vaule = FetchByte(Cycles, memory);
				A = Vaule;
				void LDASetstatus();
			} break;
			case INS_LDA_ZP:
			{
				Byte ZeroPageAddr = FetchByte(Cycles, memory);
				A = ReadByte(Cycles, ZeroPageAddr, memory);
				void LDASetstatus();
			}break;
			case INS_LDA_ZPX:
			{
				Byte ZeroPageAddr = FetchByte(Cycles, memory);
				ZeroPageAddr += X;
				Cycles--;
				A = ReadByte(Cycles, ZeroPageAddr, memory);
				void LDASetstatus();
			}break;
			case INS_JSR:
			{
				Word SubAddr = FetchWord(Cycles, memory);
				memory.WriteWord(PC - 1, SP, Cycles);
				memory[SP] = PC - 1;
				PC = SubAddr;
				Cycles--;
			}
			default:
			{
				printf("Instrcution not hnadles %d", Ins);
			} break;
			}*/
		}
	}
};


int main()
{
	Mem mem;
	CPU cpu;
	cpu.Reset(mem);
	mem[0xFFFC] = CPU::INS_JSR;
	mem[0xFFFD] = 0x42;
	mem[0xFFFE] = 0x42;
	mem[0x4242] = CPU::INS_LDA_IM;
	mem[0x4243] = 0x84;
	std::cout << static_cast<int>(mem[0x4243]) << std::endl;
	cpu.Execute(9, mem);
	return 0;
}
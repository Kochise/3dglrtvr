#ifndef swShader_Instruction_hpp
#define swShader_Instruction_hpp

#include "Operand.hpp"
#include "Instruction.hpp"

namespace swShader
{
	class PS20Assembler;

	class Instruction
	{
		friend PS20Assembler;

	public:
		enum Mnemonic
		{
			INVALID,

			PS_2_0,
			DCL,
			DCL_2D,
			DCL_CUBE,
			DCL_VOLUME,
			DEF,

			ABS,
			ADD,
			CMP,
			CRS,
			DP2ADD,
			DP3,
			DP4,
			EXP,
			FRC,
			LOG,
			LRP,
			M3X2,
			M3X3,
			M3X4,
			M4X3,
			M4X4,
			MAD,
			MAX,
			MIN,
			MOV,
			MUL,
			NOP,
			NRM,
			POW,
			RCP,
			RSQ,
			SINCOS,
			SUB,

			TEXKILL,
			TEXLD,
			TEXLDB,
			TEXLDP
		};

		enum Modifier
		{
			_NONE,

			_SAT,
			_PP
		};

		Instruction();

		~Instruction();

		Instruction *newNext();

	private:
		Mnemonic mnemonic;
		Modifier modifier;
		Operand destination;
		Operand source0;
		Operand source1;
		Operand source2;
		Operand source3;

		Instruction *next;
	};
}

#endif
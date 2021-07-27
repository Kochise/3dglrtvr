#include "Instruction.hpp"

namespace swShader
{
	Instruction::Instruction()
	{
		mnemonic = Mnemonic::INVALID;
		modifier = Modifier::_NONE;
		destination = voidOperand;
		source0 = voidOperand;
		source1 = voidOperand;
		source2 = voidOperand;
		source3 = voidOperand;

		next = 0;
	}

	Instruction::~Instruction()
	{
		delete next;
	}

	Instruction *Instruction::newNext()
	{
		delete next;
		next = new Instruction;
		return next;
	}
}
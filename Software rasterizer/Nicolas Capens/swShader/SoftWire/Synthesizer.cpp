#include "Synthesizer.hpp"

#include "Instruction.hpp"
#include "Error.hpp"
#include "String.hpp"

#include <stdlib.h>

namespace SoftWire
{
	Synthesizer::Synthesizer()
	{
		reset();
	}

	Synthesizer::~Synthesizer()
	{
	}

	void Synthesizer::reset()
	{
		encoding.reset();

		firstType = Operand::UNKNOWN;
		secondType = Operand::UNKNOWN;

		firstReg = Encoding::REG_UNKNOWN;
		secondReg = Encoding::REG_UNKNOWN;
		baseReg = Encoding::REG_UNKNOWN;
		indexReg = Encoding::REG_UNKNOWN;
		
		scale = 0;
	}

	void Synthesizer::defineLabel(const char *label)
	{
		if(encoding.label != 0)
		{
			throw INTERNAL_ERROR;   // Parser error
		}

		encoding.setLabel(label);
	}

	void Synthesizer::referenceLabel(const char *label)
	{
		if(encoding.reference != 0)
		{
			throw Error("Instruction can't have multiple references");
		}

		encoding.setReference(label);
	}

	void Synthesizer::encodeFirstOperand(const Operand &firstOperand)
	{
		if(firstType != Operand::UNKNOWN)
		{
			throw INTERNAL_ERROR;   // Instruction destination already set
		}
	
		firstType = firstOperand.type;

		if(Operand::isReg(firstType))
		{
			firstReg = firstOperand.reg;
		}
		else if(Operand::isMem(firstType))
		{
			encodeBase(firstOperand);
			encodeIndex(firstOperand);

			setScale(firstOperand.scale);
			setDisplacement(firstOperand.displacement);

			referenceLabel(firstOperand.notation);
		}
		else if(Operand::isImm(firstType))
		{
			encodeImmediate(firstOperand.value);
			referenceLabel(firstOperand.notation);
		}
		else if(Operand::isStr(firstType))
		{
			encodeLiteral(firstOperand.notation);
		}
		else if(!Operand::isVoid(firstType))
		{
			throw INTERNAL_ERROR;
		}
	}

	void Synthesizer::encodeSecondOperand(const Operand &secondOperand)
	{
		if(secondType != Operand::UNKNOWN)
		{
			throw INTERNAL_ERROR;   // Instruction source already set
		}

		secondType = secondOperand.type;

		if(Operand::isReg(secondType))
		{
			secondReg = secondOperand.reg;
		}
		else if(Operand::isMem(secondType))
		{
			encodeBase(secondOperand);
			encodeIndex(secondOperand);

			setScale(secondOperand.scale);
			setDisplacement(secondOperand.displacement);

			referenceLabel(secondOperand.notation);
		}
		else if(Operand::isImm(secondType))
		{
			encodeImmediate(secondOperand.value);
			referenceLabel(secondOperand.notation);
		}
		else if(!Operand::isVoid(secondType))
		{
			throw INTERNAL_ERROR;
		}
	}

	void Synthesizer::encodeThirdOperand(const Operand &thirdOperand)
	{
		if(Operand::isImm(thirdOperand))
		{
			encodeImmediate(thirdOperand.value);
			referenceLabel(thirdOperand.notation);
		}
		else if(!Operand::isVoid(thirdOperand))
		{
			throw INTERNAL_ERROR;
		}
	}

	void Synthesizer::encodeBase(const Operand &base)
	{
		if(baseReg != Encoding::REG_UNKNOWN)
		{
			// Base already set, use as index with scale = 1
			encodeIndex(base);
			setScale(1);
			return;
		}

		baseReg = base.baseReg;
	}

	void Synthesizer::encodeIndex(const Operand &index)
	{
		if(indexReg != Encoding::REG_UNKNOWN)
		{
			throw Error("Memory reference can't have multiple index registers");
		}

		indexReg = index.indexReg;
	}
	
	void Synthesizer::setScale(int scale)
	{
		if(this->scale != 0)
		{
			throw Error("Memory reference can't have multiple scale factors");
		}

		if(scale != 0 && scale != 1 && scale != 2 && scale != 4 && scale != 8)
		{
			throw Error("Invalid scale value '%d'", scale);
		}

		this->scale = scale;
	}

	void Synthesizer::encodeImmediate(int immediate)
	{
		if(encoding.immediate != 0xCCCCCCCC)
		{
			throw Error("Instruction can't have multiple immediate operands");
		}

		encoding.immediate = immediate;
	}

	void Synthesizer::setDisplacement(int displacement)
	{
		encoding.setDisplacement(displacement);
	}

	void Synthesizer::encodeLiteral(const char *string)
	{
		encoding.literal = strdup(string);
		encoding.format.O1 = false;   // Indicates that this is data
	}

	const Encoding &Synthesizer::encodeInstruction(const Instruction *instruction)
	{
		if(!instruction)
		{
			return encoding;
		}

		const char *format = instruction->getEncoding();

		if(!format)
		{
			throw INTERNAL_ERROR;
		}

		while(*format)
		{
			switch((format[0] << 8) | format[1])
			{
			case LOCK_PRE:
				encoding.addPrefix(0xF0);
				break;
			case CONST_PRE:
				encoding.addPrefix(0xF1);
				break;
			case REPNE_PRE:
				encoding.addPrefix(0xF2);
				break;
			case REP_PRE:
				encoding.addPrefix(0xF3);
				break;
			case OFF_PRE:
				if(!instruction->is32Bit())
				{
					encoding.addPrefix(0x66);
				}
				break;
			case ADDR_PRE:
				if(!instruction->is32Bit())
				{
					encoding.addPrefix(0x67);
				}
				break;
			case ADD_REG:
				if(encoding.format.O1)
				{
					if(Operand::isReg(firstType))
					{
						encoding.O1 += firstReg;
					}
					else if(Operand::isReg(secondType))
					{
						encoding.O1 += secondReg;
					}
					else
					{
						throw INTERNAL_ERROR;   // '+r' not compatible with operands
					}
				}
				else
				{
					throw INTERNAL_ERROR;   // '+r' needs first opcode byte
				}
				break;
			case EFF_ADDR:
				encodeModField();
				encodeRegField(instruction);
				encodeR_MField(instruction);

				encodeSibByte();

				break;
			case MOD_RM_0:
			case MOD_RM_1:
			case MOD_RM_2:
			case MOD_RM_3:
			case MOD_RM_4:
			case MOD_RM_5:
			case MOD_RM_6:
			case MOD_RM_7:
				encodeModField();
				encoding.modRM.reg = (Encoding::Reg)(format[1] - '0');
				encodeR_MField(instruction);

				encodeSibByte();

				break;
			case DWORD_IMM:
				encoding.format.I1 = true;
				encoding.format.I2 = true;
				encoding.format.I3 = true;
				encoding.format.I4 = true;
				break;
			case WORD_IMM:
				encoding.format.I1 = true;
				encoding.format.I2 = true;
				break;
			case BYTE_IMM:
				encoding.format.I1 = true;
				break;
			case BYTE_REL:
				encoding.format.I1 = true;
				encoding.relative = true;
				break;
			case DWORD_REL:
				encoding.format.I1 = true;
				encoding.format.I2 = true;
				encoding.format.I3 = true;
				encoding.format.I4 = true;
				encoding.relative = true;
				break;
			default:
				unsigned int opcode = strtoul(format, 0, 16);

				if(opcode > 0xFF)
				{
					throw INTERNAL_ERROR;
				}

				if(!encoding.format.O1)
				{
					encoding.O1 = (unsigned char)opcode;

					encoding.format.O1 = true;
				}
				else if(!encoding.format.O2 &&
				        (encoding.O1 == 0x0F ||
				         encoding.O1 == 0xD8 ||
				         encoding.O1 == 0xD9 ||
				         encoding.O1 == 0xDA ||
				         encoding.O1 == 0xDB ||
				         encoding.O1 == 0xDC ||
				         encoding.O1 == 0xDD ||
				         encoding.O1 == 0xDE ||
				         encoding.O1 == 0xDF))
				{
					encoding.O2 = encoding.O1;
					encoding.O1 = opcode;

					encoding.format.O2 = true;
				}
				else if(encoding.O1 == 0x9B)   // FWAIT
				{
					encoding.addPrefix(0x9B);   // HACK: Might not be valid for later instruction sets

					encoding.O1 = opcode;
				}
				else if(encoding.O1 == 0x0F && encoding.O2 == 0x0F)   // 3DNow! instruction, opcode as immediate
				{
					encoding.format.I1 = true;

					encoding.I1 = opcode;
				}
				else
				{
					throw INTERNAL_ERROR;
				}
			}

			format += 2;

			if(*format == ' ')	
			{
				format++;
			}
			else if(*format == '\0')
			{
				break;
			}
			else
			{
				throw INTERNAL_ERROR;
			}
		}

		return encoding;
	}

	void Synthesizer::encodeModField()
	{
		encoding.format.modRM = true;
		
		if(Operand::isReg(firstType) &&
		   (Operand::isReg(secondType) || Operand::isImm(secondType) || Operand::isVoid(secondType)))
		{
			encoding.modRM.mod = Encoding::MOD_REG;
		}
		else
		{
			if(baseReg == Encoding::REG_UNKNOWN)   // Static address
			{
				encoding.modRM.mod = Encoding::MOD_NO_DISP;
				encoding.format.D1 = true;
				encoding.format.D2 = true;
				encoding.format.D3 = true;
				encoding.format.D4 = true;
			}
			else if(encoding.reference && !encoding.displacement)
			{
				encoding.modRM.mod = Encoding::MOD_DWORD_DISP;
				encoding.format.D1 = true;
				encoding.format.D2 = true;
				encoding.format.D3 = true;
				encoding.format.D4 = true;
			}
			else if(!encoding.displacement)
			{
				encoding.modRM.mod = Encoding::MOD_NO_DISP;
			}
			else if((char)encoding.displacement == encoding.displacement)
			{
				encoding.modRM.mod = Encoding::MOD_BYTE_DISP;
				encoding.format.D1 = true;
			}
			else
			{
				encoding.modRM.mod = Encoding::MOD_DWORD_DISP;
				encoding.format.D1 = true;
				encoding.format.D2 = true;
				encoding.format.D3 = true;
				encoding.format.D4 = true;
			}
		}
	}

	void Synthesizer::encodeR_MField(const Instruction *instruction)
	{
		if(Operand::isReg(instruction->getFirstOperand()) &&
		   Operand::isR_M(instruction->getSecondOperand()))
		{
			if(Operand::isMem(secondType))
			{
				if(baseReg == Encoding::REG_UNKNOWN)
				{
					encoding.modRM.r_m = Encoding::EBP;   // Static address
				}
				else
				{
					encoding.modRM.r_m = baseReg;
				}
			}
			else if(Operand::isReg(secondType))
			{
				encoding.modRM.r_m = secondReg;
			}
			else
			{
				throw INTERNAL_ERROR;   // Syntax error should be detected by parser
			}
		}
		else if(Operand::isR_M(instruction->getFirstOperand()) &&
		        Operand::isReg(instruction->getSecondOperand()))
		{
			if(Operand::isMem(firstType))
			{
				if(baseReg == Encoding::REG_UNKNOWN)
				{
					encoding.modRM.r_m = Encoding::EBP;   // Static address
				}
				else
				{
					encoding.modRM.r_m = baseReg;
				}
			}
			else if(Operand::isReg(firstType))
			{
				encoding.modRM.r_m = firstReg;
			}
			else
			{
				throw INTERNAL_ERROR;   // Syntax error should be detected by parser
			}
		}
		else
		{
			if(Operand::isMem(firstType))
			{
				if(baseReg != Encoding::REG_UNKNOWN)
				{
					encoding.modRM.r_m = baseReg;
				}
				else
				{
					encoding.modRM.r_m = Encoding::EBP;   // Displacement only
				}
			}
			else if(Operand::isReg(firstType))
			{
				encoding.modRM.r_m = firstReg;
			}
			else
			{
				throw INTERNAL_ERROR;   // Syntax error should be caught by parser
			}
		}
	}

	void Synthesizer::encodeRegField(const Instruction *instruction)
	{
		if(Operand::isReg(instruction->getFirstOperand()) &&
		   Operand::isR_M(instruction->getSecondOperand()))
		{
			encoding.modRM.reg = firstReg;
		}
		else if(Operand::isR_M(instruction->getFirstOperand()) &&
		        Operand::isReg(instruction->getSecondOperand()))
		{
			encoding.modRM.reg = secondReg;
		}
		else if(Operand::isReg(instruction->getFirstOperand()) &&
		        Operand::isImm(instruction->getSecondOperand()))   // IMUL working on the same register
		{
			encoding.modRM.reg = firstReg;
		}
		else
		{
			throw INTERNAL_ERROR;
		}
	}

	void Synthesizer::encodeSibByte()
	{
		if(scale == 0 && indexReg == Encoding::REG_UNKNOWN)
		{
			if((baseReg == Encoding::REG_UNKNOWN) ||
			   (encoding.modRM.r_m != Encoding::ESP && encoding.modRM.r_m != Encoding::EBP))
			{
				if(encoding.format.SIB)
				{
					throw INTERNAL_ERROR;
				}

				return;   // No SIB byte needed
			}
		}

		encoding.format.SIB = true;

		encoding.modRM.r_m = Encoding::ESP;   // Indicates use of SIB in mod R/M

		if(baseReg == Encoding::EBP && encoding.modRM.mod == Encoding::MOD_NO_DISP)
		{
			encoding.modRM.mod = Encoding::MOD_BYTE_DISP;

			encoding.format.D1 = true;
		}

		if(indexReg == Encoding::ESP)
		{
			if(scale != 1)
			{
				throw Error("ESP can't be scaled index in memory reference");
			}
			else   // Switch base and index
			{
				Encoding::Reg tempReg;

				tempReg = indexReg;
				indexReg = baseReg;
				baseReg = tempReg;
			}
		}

		if(baseReg == Encoding::REG_UNKNOWN)
		{
			encoding.SIB.base = Encoding::EBP;   // No Base

			encoding.modRM.mod = Encoding::MOD_NO_DISP;
			encoding.format.D1 = true;
			encoding.format.D2 = true;
			encoding.format.D3 = true;
			encoding.format.D4 = true;
		}
		else
		{
			encoding.SIB.base = baseReg;
		}

		if(indexReg != Encoding::REG_UNKNOWN)
		{
			encoding.SIB.index = indexReg;
		}
		else
		{
			encoding.SIB.index = Encoding::ESP;
		}

		switch(scale)
		{
		case 0:
		case 1:
			encoding.SIB.scale = Encoding::SCALE_1;
			break;
		case 2:
			encoding.SIB.scale = Encoding::SCALE_2;
			break;
		case 4:
			encoding.SIB.scale = Encoding::SCALE_4;
			break;
		case 8:
			encoding.SIB.scale = Encoding::SCALE_8;
			break;
		default:
			throw INTERNAL_ERROR;
		}
	}
}

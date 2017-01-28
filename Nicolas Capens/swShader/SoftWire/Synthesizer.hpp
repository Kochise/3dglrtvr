#ifndef SoftWire_Synthesizer_hpp
#define SoftWire_Synthesizer_hpp

#include "Encoding.hpp"
#include "Operand.hpp"

namespace SoftWire
{
	class Instruction;

	class Synthesizer
	{
	public:
		Synthesizer();

		~Synthesizer();

		void reset();

		void defineLabel(const char *label);

		void encodeFirstOperand(const Operand &firstOperand);
		void encodeSecondOperand(const Operand &secondOperand);
		void encodeThirdOperand(const Operand &thirdOperand);

		void encodeImmediate(int i);
		void encodeLiteral(const char *string);

		const Encoding &encodeInstruction(const Instruction *instruction);

	private:
		Encoding encoding;

		Operand::Type firstType;
		Operand::Type secondType;

		Encoding::Reg firstReg;
		Encoding::Reg secondReg;
		Encoding::Reg baseReg;
		Encoding::Reg indexReg;

		int scale;

		void encodeBase(const Operand &base);
		void encodeIndex(const Operand &index);

		void setScale(int scale);
		void setDisplacement(int displacement);

		void referenceLabel(const char *label);

		void encodeModField();
		void encodeR_MField(const Instruction *instruction);
		void encodeRegField(const Instruction *instruction);

		void encodeSibByte();
	};
}

#endif   // SoftWire_Synthesizer_hpp

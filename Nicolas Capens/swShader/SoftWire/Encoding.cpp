#include "Encoding.hpp"

#include "Error.hpp"
#include "String.hpp"

namespace SoftWire
{
	Encoding::Encoding()
	{
		label = 0;
		reference = 0;

		reset();

		O1 = 0xCC;   // Breakpoint
		format.O1 = true;
	}

	Encoding::Encoding(const Encoding &encoding)
	{
		format = encoding.format;

		label = strdup(encoding.label);
		reference = strdup(encoding.reference);
		relative = encoding.relative;

		P1 = encoding.P1;
		P2 = encoding.P2;
		P3 = encoding.P3;
		P4 = encoding.P4;
		O2 = encoding.O2;
		O1 = encoding.O1;
		modRM.b = encoding.modRM.b;
		SIB.b = encoding.SIB.b;

		immediate = encoding.immediate;
		displacement = encoding.displacement;
	}

	Encoding::~Encoding()
	{
		delete[] label;
		delete[] reference;
	}

	Encoding &Encoding::operator=(const Encoding &encoding)
	{
		format = encoding.format;

		if (encoding.label) label = strdup(encoding.label);
		if (encoding.reference) reference = strdup(encoding.reference);
		relative = encoding.relative;

		P1 = encoding.P1;
		P2 = encoding.P2;
		P3 = encoding.P3;
		P4 = encoding.P4;
		O2 = encoding.O2;
		O1 = encoding.O1;
		modRM.b = encoding.modRM.b;
		SIB.b = encoding.SIB.b;

		immediate = encoding.immediate;
		displacement = encoding.displacement;

		return *this;
	}

	void Encoding::reset()
	{
		delete[] label;
		delete[] reference;

		label = 0;
		reference = 0;
		relative = false;

		format.P1 = false;
		format.P2 = false;
		format.P3 = false;
		format.P4 = false;
		format.O2 = false;
		format.O1 = false;
		format.modRM = false;
		format.SIB = false;
		format.D1 = false;
		format.D2 = false;
		format.D3 = false;
		format.D4 = false;
		format.I1 = false;
		format.I2 = false;
		format.I3 = false;
		format.I4 = false;

		P1 = 0xCC;
		P2 = 0xCC;
		P3 = 0xCC;
		P4 = 0xCC;
		O2 = 0xCC;
		O1 = 0xCC;
		modRM.b = 0xCC;
		SIB.b = 0xCC;
		D1 = 0xCC;
		D2 = 0xCC;
		D3 = 0xCC;
		D4 = 0xCC;
		I1 = 0xCC;
		I2 = 0xCC;
		I3 = 0xCC;
		I4 = 0xCC;

		address = 0;
	}

	const char *Encoding::getLabel() const
	{
		return label;
	}

	const char *Encoding::getReference() const
	{
		if(P1 != 0xF1)
		{
			return reference;
		}
		else
		{
			return 0;
		}
	}

	const char *Encoding::getLiteral() const
	{
		if(!format.O1)
		{
			return literal;
		}
		else
		{
			return 0;
		}
	}

	int Encoding::getImmediate() const
	{
		return immediate;
	}

	int Encoding::length(const unsigned char *output) const
	{
		return writeCode((unsigned char*)output, false);
	}

	void Encoding::addPrefix(unsigned char p)
	{
		if(!format.P1)
		{
			P1 = p;
			format.P1 = true;;
		}
		else if(!format.P2)
		{
			P2 = p;
			format.P2 = true;
		}
		else if(!format.P3)
		{
			P3 = p;
			format.P3 = true;
		}
		else if(!format.P4)
		{
			P4 = p;
			format.P4 = true;
		}
		else
		{
			throw Error("Too many prefixes in opcode");
		}
	}

	void Encoding::setImmediate(int immediate)
	{
		this->immediate = immediate;
	}

	void Encoding::setDisplacement(int displacement)
	{
		this->displacement = displacement;
	}

	void Encoding::addDisplacement(int displacement)
	{
		this->displacement += displacement;
	}

	void Encoding::setJumpOffset(int offset)
	{
		if((char)offset != offset && !format.I2)
		{
			throw Error("Jump offset range too big");
		}

		this->immediate = offset;
	}

	void Encoding::setCallOffset(int offset)
	{
		if(!(format.I1 && format.I2 && format.I3 && format.I4))
		{
			throw Error("Call offset should be 32-bit");
		}

		this->immediate = offset;
	}

	void Encoding::setLabel(const char *label)
	{
		if(label != 0)
		{
			this->label = strdup(label);
		}
	}

	void Encoding::setReference(const char *label)
	{
		if(label != 0)
		{
			reference = strdup(label);
		}
	}

	bool Encoding::relativeReference() const
	{
		return relative;
	}

	bool Encoding::absoluteReference() const
	{
		return !relativeReference();
	}

	bool Encoding::hasDisplacement() const
	{
		return format.D1 || format.D2 || format.D3 || format.D4;
	}

	bool Encoding::hasImmediate() const
	{
		return format.I1 || format.I2 || format.I3 || format.I4;
	}

	void Encoding::setAddress(const unsigned char *address)
	{
		this->address = address;
	}

	const unsigned char *Encoding::getAddress() const
	{
		return address;
	}

	int Encoding::writeCode(unsigned char *buffer, bool write) const
	{
		unsigned char *start = buffer;

		#define OUTPUT_BYTE(x) if(write) {*buffer++ = (x);} else {buffer++;}

		if(P1 == 0xF1)   // Special 'instructions', indicated by INT01 prefix byte
		{
			if(O1 == 0x00 && literal)   // String constant
			{
				for(int i = 0; literal[i]; i++)
				{
					OUTPUT_BYTE(literal[i]);
				}
					
				OUTPUT_BYTE('\0');
				buffer += align(buffer, 4, write);
			}		
			else if(O1 == 0x90 && immediate)   // ALIGN
			{
				if(immediate > 256)
				{
					throw Error("ALIGN value too big: %d bytes", immediate);
				}

				buffer += align(buffer, immediate, write);
			}
			else if((O1 == 0x01 || O1 == 0x02 || O1 == 0x04) && displacement)   // Array
			{
				if(O1 == 0x04)
				{
					buffer += align(buffer, 4, write);
				}
				else if(O1 == 0x02)
				{
					buffer += align(buffer, 2, write);
				}

				for(int i = 0; i < O1 * displacement; i++)
				{
					OUTPUT_BYTE(0xCC);   // INT3
				}
			}
			else
			{
				if(format.I4)
				{
					buffer += align(buffer, 4, write);
				}
				else if(format.I2)
				{
					buffer += align(buffer, 2, write);
				}

				if(format.I1)		OUTPUT_BYTE(I1);
				if(format.I2)		OUTPUT_BYTE(I2);
				if(format.I3)		OUTPUT_BYTE(I3);
				if(format.I4)		OUTPUT_BYTE(I4);
			}
		}
		else   // Normal instructions
		{
			if(format.P1)		OUTPUT_BYTE(P1);
			if(format.P2)		OUTPUT_BYTE(P2);
			if(format.P3)		OUTPUT_BYTE(P3);
			if(format.P4)		OUTPUT_BYTE(P4);
			if(format.O2)		OUTPUT_BYTE(O2);
			if(format.O1)		OUTPUT_BYTE(O1);
			if(format.modRM)	OUTPUT_BYTE(modRM.b);
			if(format.SIB)		OUTPUT_BYTE(SIB.b);
			if(format.D1)		OUTPUT_BYTE(D1);
			if(format.D2)		OUTPUT_BYTE(D2);
			if(format.D3)		OUTPUT_BYTE(D3);
			if(format.D4)		OUTPUT_BYTE(D4);
			if(format.I1)		OUTPUT_BYTE(I1);
			if(format.I2)		OUTPUT_BYTE(I2);
			if(format.I3)		OUTPUT_BYTE(I3);
			if(format.I4)		OUTPUT_BYTE(I4);
		}

		#undef OUTPUT_BYTE

		return buffer - start;
	}

	int Encoding::align(unsigned char *buffer, int alignment, bool write)
	{
		unsigned char *start = buffer;

		int padding = alignment - (int)buffer % alignment;

		if(padding == alignment)
		{
			padding = 0;
		}

		for(int i = 0; i < padding; i++)
		{
			if(write)
			{
				*buffer = 0x90;   // NOP
			}

			buffer++;
		}

		return buffer - start;
	}

	int Encoding::printCode(char *buffer) const
	{
		char *start = buffer;

		if(format.P1)		{sprintf(buffer, "%.2X ", P1);		buffer += 3;}
		if(format.P2)		{sprintf(buffer, "%.2X ", P2);		buffer += 3;}
		if(format.P3)		{sprintf(buffer, "%.2X ", P3);		buffer += 3;}
		if(format.P4)		{sprintf(buffer, "%.2X ", P4);		buffer += 3;}
		if(format.O2)		{sprintf(buffer, "%.2X ", O2);		buffer += 3;}
		if(format.O1)		{sprintf(buffer, "%.2X ", O1);		buffer += 3;}
		if(format.modRM)	{sprintf(buffer, "%.2X ", modRM.b);	buffer += 3;}
		if(format.SIB)		{sprintf(buffer, "%.2X ", SIB.b);	buffer += 3;}
		if(format.D1)		{sprintf(buffer, "%.2X ", D1);		buffer += 3;}
		if(format.D2)		{sprintf(buffer, "%.2X ", D2);		buffer += 3;}
		if(format.D3)		{sprintf(buffer, "%.2X ", D3);		buffer += 3;}
		if(format.D4)		{sprintf(buffer, "%.2X ", D4);		buffer += 3;}
		if(format.I1)		{sprintf(buffer, "%.2X ", I1);		buffer += 3;}
		if(format.I2)		{sprintf(buffer, "%.2X ", I2);		buffer += 3;}
		if(format.I3)		{sprintf(buffer, "%.2X ", I3);		buffer += 3;}
		if(format.I4)		{sprintf(buffer, "%.2X ", I4);		buffer += 3;}

		sprintf(buffer++, "\n");

		return buffer - start;
	}
}

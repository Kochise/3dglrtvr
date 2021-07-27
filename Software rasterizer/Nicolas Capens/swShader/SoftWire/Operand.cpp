#include "Operand.hpp"

#include "String.hpp"

namespace SoftWire
{
	const Specifier Specifier::specifierSet[] =
	{
		{UNKNOWN,	""},

		{NEAR,		"NEAR"},
		{SHORT,		"SHORT"},
	//	{FAR,		"FAR"},

		{BYTE,		"BYTE"},
		{WORD,		"WORD"},
		{DWORD,		"DWORD"},
	//	{TWORD,		"TWORD"},
		{QWORD,		"QWORD"},
		{MMWORD,	"MMWORD"},
		{XMMWORD,	"XMMWORD"},
		{XWORD,		"XWORD"},
		{OWORD,		"OWORD"},

		{PTR,		"PTR"}
	};

	Specifier::Type Specifier::scan(const char *string)
	{
		if(string)
		{
			for(int i = 0; i < sizeof(specifierSet) / sizeof(Specifier); i++)
			{
				if(stricmp(string, specifierSet[i].notation) == 0)
				{
					return specifierSet[i].type;
				}		
			}
		}

		return UNKNOWN;
	}

	bool Operand::isSubtypeOf(Type type, Type baseType)
	{
		return (type & baseType) == type;
	}

	bool Operand::isSubtypeOf(Type baseType) const
	{
		return isSubtypeOf(type, baseType);
	}

	const char *Operand::string() const
	{
		static char string[256];

		if(isVoid(type))
		{
			return 0;
		}
		else if(isImm(type))
		{
			_snprintf(string, 255, "0x%X", value);
		}
		else if(isReg(type))
		{
			return regName();
		}
		else if(isMem(type))
		{
			switch(type)
			{
			case MEM8:
				_snprintf(string, 255, "byte ptr [");
				break;
			case MEM16:
				_snprintf(string, 255, "word ptr [");
				break;
			case MEM32:
				_snprintf(string, 255, "dword ptr [");
				break;
			case MEM64:
				_snprintf(string, 255, "qword ptr [");
				break;
			case MEM128:
				_snprintf(string, 255, "xmmword ptr [");
				break;
			case MEM:
			default:
				_snprintf(string, 255, "byte ptr [");
			}

			if(baseReg != Encoding::REG_UNKNOWN)
			{
				_snprintf(string, 255, "%s%s", string, regName());

				if(indexReg != Encoding::REG_UNKNOWN)
				{
					_snprintf(string, 255, "%s + ", string);
				}
			}

			if(indexReg != Encoding::REG_UNKNOWN)
			{
				_snprintf(string, 255, "%s%s", string, indexName());
			}

			switch(scale)
			{
			case 0:
			case 1:
				break;
			case 2:
				_snprintf(string, 255, "%s * 2", string);
				break;
			case 4:
				_snprintf(string, 255, "%s * 4", string);
				break;
			case 8:
				_snprintf(string, 255, "%s * 8", string);
				break;
			default:
				throw INTERNAL_ERROR;
			}

			if(displacement)
			{
				if(baseReg != Encoding::REG_UNKNOWN ||
				   indexReg != Encoding::REG_UNKNOWN)
				{
					_snprintf(string, 255, "%s + ", string);
				}

				_snprintf(string, 255, "%s0x%X", string, displacement);
			}

			_snprintf(string, 255, "%s]", string);
		}
		else if(isStr(type))
		{
			return notation;
		}
		else
		{
			throw INTERNAL_ERROR;
		}

		return strlwr(string);
	}

	bool Operand::isVoid(Type type)
	{
		return type == VOID;
	}

	bool Operand::isImm(Type type)
	{
		return (type & IMM) == type;
	}

	bool Operand::isReg(Type type)
	{
		return (type & REG) == type;
	}

	bool Operand::isMem(Type type)
	{
		return (type & MEM) == type;
	}

	bool Operand::isR_M(Type type)
	{
		return (type & R_M) == type;
	}

	bool Operand::isStr(Type type)
	{
		return (type & STR) == type;
	}

	bool Operand::isVoid(const Operand &operand)
	{
		return isVoid(operand.type);
	}

	bool Operand::isImm(const Operand &operand)
	{
		return isImm(operand.type);
	}

	bool Operand::isReg(const Operand &operand)
	{
		return isReg(operand.type);
	}

	bool Operand::isMem(const Operand &operand)
	{
		return isMem(operand.type);
	}

	bool Operand::isR_M(const Operand &operand)
	{
		return isR_M(operand.type);
	}

	bool Operand::isStr(const Operand &operand)
	{
		return isStr(operand.type);
	}

	const Operand Operand::registerSet[] =
	{
		{VOID,		""},

		{AL,		"al", 0},
		{CL,		"cl", 1},
		{REG8,		"dl", 2},
		{REG8,		"bl", 3},
		{REG8,		"ah", 4},
		{REG8,		"ch", 5},
		{REG8,		"dh", 6},
		{REG8,		"bh", 7},

		{AX,		"ax", 0},
		{CX,		"cx", 1},
		{DX,		"dx", 2},
		{REG16,		"bx", 3},
		{REG16,		"sp", 4},
		{REG16,		"bp", 5},
		{REG16,		"si", 6},
		{REG16,		"di", 7},

		{EAX,		"eax", 0},
		{ECX,		"ecx", 1},
		{REG32,		"edx", 2},
		{REG32,		"ebx", 3},
		{REG32,		"esp", 4},
		{REG32,		"ebp", 5},
		{REG32,		"esi", 6},
		{REG32,		"edi", 7},

		{ST0,		"st",  0},
		{ST0,		"st0", 0},
		{FPUREG,	"st1", 1},
		{FPUREG,	"st2", 2},
		{FPUREG,	"st3", 3},
		{FPUREG,	"st4", 4},
		{FPUREG,	"st5", 5},
		{FPUREG,	"st6", 6},
		{FPUREG,	"st7", 7},

		{MMREG,		"mm0", 0},
		{MMREG,		"mm1", 1},
		{MMREG,		"mm2", 2},
		{MMREG,		"mm3", 3},
		{MMREG,		"mm4", 4},
		{MMREG,		"mm5", 5},
		{MMREG,		"mm6", 6},
		{MMREG,		"mm7", 7},

		{XMMREG,	"xmm0", 0},
		{XMMREG,	"xmm1", 1},
		{XMMREG,	"xmm2", 2},
		{XMMREG,	"xmm3", 3},
		{XMMREG,	"xmm4", 4},
		{XMMREG,	"xmm5", 5},
		{XMMREG,	"xmm6", 6},
		{XMMREG,	"xmm7", 7}
	};

	OperandREG Operand::scanReg(const char *string)
	{
		if(string)
		{
			for(int i = 0; i < sizeof(registerSet) / sizeof(Operand); i++)
			{
				if(stricmp(string, registerSet[i].notation) == 0)
				{
					return registerSet[i];
				}
			}
		}

		return NOT_FOUND;
	}

	const char *Operand::regName() const
	{
		for(int i = 0; i < sizeof(registerSet) / sizeof(Operand); i++)
		{
			if(reg == registerSet[i].reg)
			{
				if(isSubtypeOf(MEM) && registerSet[i].isSubtypeOf(REG32) ||
				   isSubtypeOf(registerSet[i].type) && reg == registerSet[i].reg)
				{
					return registerSet[i].notation;
				}
			}
		}

		throw INTERNAL_ERROR;
	}

	const char *Operand::indexName() const
	{
		for(int i = 0; i < sizeof(registerSet) / sizeof(Operand); i++)
		{
			if(indexReg == registerSet[i].reg && registerSet[i].isSubtypeOf(REG32))
			{
				return registerSet[i].notation;
			}
		}

		throw INTERNAL_ERROR;
	}

	const Operand Operand::syntaxSet[] =
	{
		{VOID,		""},

		{ONE,		"1"},
		{IMM,		"imm"},
		{IMM8,		"imm8"},
		{IMM16,		"imm16"},
		{IMM32,		"imm32"},

		{AL,		"AL"},
		{AX,		"AX"},
		{EAX,		"EAX"},
		{DX,		"DX"},
		{CL,		"CL"},
		{CX,		"CX"},
		{ECX,		"ECX"},
		{ST0,		"ST0"},

		{REG8,		"reg8"},
		{REG16,		"reg16"},
		{REG32,		"reg32"},
		{FPUREG,	"fpureg"},
		{MMREG,		"mmreg"},
		{XMMREG,	"xmmreg"},

		{MEM,		"mem"},
		{MEM8,		"mem8"},
		{MEM16,		"mem16"},
		{MEM32,		"mem32"},
		{MEM64,		"mem64"},
		{MEM128,	"mem128"},

		{R_M8,		"r/m8"},
		{R_M16,		"r/m16"},
		{R_M32,		"r/m32"},
		{R_M64,		"r/m64"},
		{R_M128,	"r/m128"},

		{XMM32,		"xmm32"},
		{XMM64,		"xmm64"},

		{STR,		"str"}
	};

	Operand::Type Operand::scanSyntax(const char *string)
	{
		if(string)
		{
			for(int i = 0; i < sizeof(registerSet) / sizeof(Operand); i++)
			{
				if(stricmp(string, syntaxSet[i].notation) == 0)
				{
					return syntaxSet[i].type;
				}
			}
		}

		return UNKNOWN;
	}

	const Operand Operand::INIT = {Operand::VOID};
	const Operand Operand::NOT_FOUND = {Operand::UNKNOWN};
}

#include "PS20Assembler.hpp"

#include "Instruction.hpp"
#include "Error.hpp"
#include "Texture.hpp"

#include <float.h>

namespace swShader
{
	float4 PS20Assembler::r[12];
	float4 PS20Assembler::t[8];
	float4 PS20Assembler::s[16];
	float4 PS20Assembler::v[2];
	float4 PS20Assembler::c[32];
	float4 PS20Assembler::oC[4];
	float4 PS20Assembler::oDepth;

	PS20Assembler::PS20Assembler()
	{
		tmp0 = Operand(Operand::VIRTUAL_REGISTER, 0);
		tmp1 = Operand(Operand::VIRTUAL_REGISTER, 1);
		tmp2 = Operand(Operand::VIRTUAL_REGISTER, 2);
		tmp3 = Operand(Operand::VIRTUAL_REGISTER, 3);
		tmp4 = Operand(Operand::VIRTUAL_REGISTER, 4);
		tmp5 = Operand(Operand::VIRTUAL_REGISTER, 5);
		tmp6 = Operand(Operand::VIRTUAL_REGISTER, 6);
		tmp7 = Operand(Operand::VIRTUAL_REGISTER, 7);

		for(int i = 0; i < 8; i++) xmm[i].type = Operand::VOID;

		intermediate = new Instruction();
		instruction = intermediate;
	}

	PS20Assembler::~PS20Assembler()
	{
		delete intermediate;
		intermediate = 0;
	}

	void (*PS20Assembler::shader())()
	{
		return callable();
	}

	void PS20Assembler::setConstant(int index, const float value[4])
	{
		if(index < 0 || index >= 32) throw Error("Constant register index out of [0, 31] range: %d", index);

		c[index][0] = value[0];
		c[index][1] = value[1];
		c[index][2] = value[2];
		c[index][3] = value[3];
	}

	void PS20Assembler::setMnemonic(Instruction::Mnemonic mnemonic)
	{
		instruction->mnemonic = mnemonic;
	}

	void PS20Assembler::setModifier(Instruction::Modifier modifier)
	{
		instruction->modifier = modifier;
	}

	void PS20Assembler::setDestination(const Operand &operand)
	{
		instruction->destination = operand;
	}

	void PS20Assembler::setSource0(const Operand &operand)
	{
		instruction->source0 = operand;
	}

	void PS20Assembler::setSource1(const Operand &operand)
	{
		instruction->source1 = operand;
	}

	void PS20Assembler::setSource2(const Operand &operand)
	{
		instruction->source2 = operand;
	}

	void PS20Assembler::setSource3(const Operand &operand)
	{
		instruction->source3 = operand;
	}

	void PS20Assembler::newInstruction()
	{
		instruction = instruction->newNext();
	}

	void PS20Assembler::encode()
	{
		if(!intermediate) throw INTERNAL_ERROR;

		#ifndef NDEBUG
			setEchoFile("Shader.asm");
		#endif

		try
		{
			pushad();

			mov(ecx, dword_ptr [&lx]);
			cmp(ecx, dword_ptr [&rx]);
			jge("return");

				setupInterpolants();

			mov(edi, dword_ptr [&colorBuffer]);
			mov(edx, dword_ptr [&depthBuffer]);

		label("scanlineLoop");

				pixel();

			inc(ecx);
			cmp(ecx, dword_ptr [&rx]);
			jnge("scanlineLoop");

		label("endScanline");

			emms();

		label("return");
			popad();
			ret();
		}
		catch(const Error &error)
		{
			throw Error("Fatal pixel shader assembler error: ") << error;
		}
		catch(...)
		{
			throw INTERNAL_ERROR;
		}
	}

	void PS20Assembler::setupInterpolants()
	{
		for(int i = 0; i < 8; i++) xmm[i].type = Operand::VOID;

		v0Declared = false;
		v1Declared = false;

		t0Declared = false;
		t1Declared = false;
		t2Declared = false;
		t3Declared = false;
		t4Declared = false;
		t5Declared = false;
		t6Declared = false;
		t7Declared = false;

		instruction = intermediate;

		if(instruction->mnemonic == Instruction::PS_2_0)
		{
			PS_2_0();
		}
		else
		{
			throw Error("First shader instruction should be PS_2_0");
		}

		instruction = instruction->next;

		while(instruction)
		{
			const Operand &dest = instruction->destination;

			if(instruction->mnemonic == Instruction::DCL)			DCL(dest);
			if(instruction->mnemonic == Instruction::DCL_2D)		DCL_2D(dest);
			if(instruction->mnemonic == Instruction::DCL_CUBE)		DCL_CUBE(dest);
			if(instruction->mnemonic == Instruction::DCL_VOLUME)	DCL_VOLUME(dest);

			instruction = instruction->next;
		};

		if(v0Declared)
		{
			movaps(xmm0, xmmword_ptr [&C]);
			movaps(xmmword_ptr [&v[0]], xmm0);
		}

		if(v1Declared)
		{
			movaps(xmm0, xmmword_ptr [&L]);
			movaps(xmmword_ptr [&v[1]], xmm0);
		}

		if(t0Declared)
		{
			movaps(xmm0, xmmword_ptr [&T[0]]);
			movaps(xmmword_ptr [&t[0]], xmm0);
		}
		
		if(t1Declared)
		{
			movaps(xmm0, xmmword_ptr [&T[1]]);
			movaps(xmmword_ptr [&t[1]], xmm0);
		}

		if(t2Declared)
		{
			movaps(xmm0, xmmword_ptr [&T[2]]);
			movaps(xmmword_ptr [&t[2]], xmm0);
		}

		if(t3Declared)
		{
			movaps(xmm0, xmmword_ptr [&T[3]]);
			movaps(xmmword_ptr [&t[3]], xmm0);
		}

		if(t4Declared)
		{
			movaps(xmm0, xmmword_ptr [&T[4]]);
			movaps(xmmword_ptr [&t[4]], xmm0);
		}

		if(t5Declared)
		{
			movaps(xmm0, xmmword_ptr [&T[5]]);
			movaps(xmmword_ptr [&t[5]], xmm0);
		}

		if(t6Declared)
		{
			movaps(xmm0, xmmword_ptr [&T[6]]);
			movaps(xmmword_ptr [&t[6]], xmm0);
		}

		if(t7Declared)
		{
			movaps(xmm0, xmmword_ptr [&T[7]]);
			movaps(xmmword_ptr [&t[7]], xmm0);
		}

		// Reset to detect usage before declaration
		v0Declared = false;
		v1Declared = false;

		t0Declared = false;
		t1Declared = false;
		t2Declared = false;
		t3Declared = false;
		t4Declared = false;
		t5Declared = false;
		t6Declared = false;
		t7Declared = false;

		movss(xmm0, dword_ptr [&w]);
		movss(dword_ptr [&RHW], xmm0);

		movss(xmm0, dword_ptr [&z]);
		movss(dword_ptr [&Z], xmm0);
	}

	void PS20Assembler::pixel()
	{
		instruction = intermediate;

		depthTest();

		while(instruction)
		{
			const Operand &dst = instruction->destination;
			const Operand &dest = instruction->destination;
			const Operand &src = instruction->source0;
			const Operand &src0 = instruction->source0;
			const Operand &src1 = instruction->source1;
			const Operand &src2 = instruction->source2;
			const Operand &src3 = instruction->source3;
			const Operand &fValue1 = instruction->source0;
			const Operand &fValue2 = instruction->source1;
			const Operand &fValue3 = instruction->source2;
			const Operand &fValue4 = instruction->source3;

			switch(instruction->mnemonic)
			{
			case Instruction::PS_2_0:														break;

			case Instruction::DCL:			DCL(dest);										break;
			case Instruction::DCL_2D:		DCL_2D(dest);									break;
			case Instruction::DCL_CUBE:		DCL_CUBE(dest);									break;
			case Instruction::DCL_VOLUME:	DCL_VOLUME(dest);								break;

			case Instruction::DEF:			DEF(dest, fValue1, fValue2, fValue3, fValue4);	break;

			case Instruction::ABS:			ABS(dst, src);									break;
			case Instruction::ADD:			ADD(dst, src0, src1);							break;
			case Instruction::CMP:			CMP(dst, src0, src1, src2);						break;
			case Instruction::CRS:			CRS(dst, src0, src1);							break;
			case Instruction::DP2ADD:		DP2ADD(dst, src0, src1, src2);					break;
			case Instruction::DP3:			DP3(dst, src0, src1);							break;
			case Instruction::DP4:			DP4(dst, src0, src1);							break;
			case Instruction::EXP:			EXP(dst, src);									break;
			case Instruction::FRC:			FRC(dst, src);									break;
			case Instruction::LOG:			LOG(dst, src);									break;
			case Instruction::LRP:			LRP(dst, src0, src1, src2);						break;
			case Instruction::M3X2:			M3X2(dst, src0, src1);							break;
			case Instruction::M3X3:			M3X3(dst, src0, src1);							break;
			case Instruction::M3X4:			M3X4(dst, src0, src1);							break;
			case Instruction::M4X3:			M4X3(dst, src0, src1);							break;
			case Instruction::M4X4:			M4X4(dst, src0, src1);							break;
			case Instruction::MAD:			MAD(dst, src0, src1, src2);						break;
			case Instruction::MAX:			MAX(dst, src0, src1);							break;
			case Instruction::MIN:			MIN(dst, src0, src1);							break;
			case Instruction::MOV:			MOV(dst, src);									break;
			case Instruction::MUL:			MUL(dst, src0, src1);							break;
			case Instruction::NOP:			NOP();											break;
			case Instruction::NRM:			NRM(dst, src);									break;
			case Instruction::POW:			POW(dst, src0, src1);							break;
			case Instruction::RCP:			RCP(dst, src);									break;
			case Instruction::RSQ:			RSQ(dst, src);									break;
			case Instruction::SINCOS:		SINCOS(dst, src0, src1, src2);					break;
			case Instruction::SUB:			SUB(dst, src0, src1);							break;

			case Instruction::TEXKILL:		TEXKILL(src);									break;
			case Instruction::TEXLD:		TEXLD(dst, src0, src1);							break;
			case Instruction::TEXLDB:		TEXLDB(dst, src0, src1);						break;
			case Instruction::TEXLDP:		TEXLDP(dst, src0, src1);						break;
			case Instruction::INVALID:														break;
			default:
				throw INTERNAL_ERROR;
			}

			freeAllTemp();

			annotate("\n");

			instruction = instruction->next;
		}

		writeOC0();

	label("texkill");

		interpolate();
	}

	void PS20Assembler::depthTest()
	{
		movss(xmmreg(tmp), dword_ptr [&Z]);
		comiss(xmmreg(tmp), dword_ptr [edx+4*ecx]);
		
		jnc("texkill");

		movss(dword_ptr [edx+4*ecx], xmmreg(tmp));

		freeAllTemp();
	}

	void PS20Assembler::writeOC0()
	{
		static const float4 _255 = {255, 255, 255, 255};

		movaps(xmmreg(tmp), r_m128(oC0Register));
		mulps(xmmreg(tmp), xmmword_ptr [&_255]);
		cvtps2pi(mm0, xmmreg(tmp));
		movhlps(xmmreg(tmp), xmmreg(tmp));
		cvtps2pi(mm1, xmmreg(tmp));
		packssdw(mm0, mm1);
		pshufw(mm0, mm0, 0xC6);
		packuswb(mm0, mm0);
		movd(dword_ptr [edi+4*ecx], mm0);

		freeAllTemp();
	}

	void PS20Assembler::interpolate()
	{
		// Vertex color/light interpolation
		if(v0Declared)
		{
			addps(xmmreg(v0Register), xmmword_ptr [&dC_dx]);
		}

		if(v1Declared)
		{
			addps(xmmreg(v1Register), xmmword_ptr [&dC_dx]);
		}

		// Texture coordinate interpolation
		if(t0Declared)
		{
			addps(xmmreg(t0Register), xmmword_ptr [&dT_dx[0]]);
		}

		if(t1Declared)
		{
			addps(xmmreg(t1Register), xmmword_ptr [&dT_dx[1]]);
		}

		if(t2Declared)
		{
			addps(xmmreg(t2Register), xmmword_ptr [&dT_dx[2]]);
		}

		if(t3Declared)
		{
			addps(xmmreg(t3Register), xmmword_ptr [&dT_dx[3]]);
		}

		if(t4Declared)
		{
			addps(xmmreg(t4Register), xmmword_ptr [&dT_dx[4]]);
		}

		if(t5Declared)
		{
			addps(xmmreg(t5Register), xmmword_ptr [&dT_dx[5]]);
		}

		if(t6Declared)
		{
			addps(xmmreg(t6Register), xmmword_ptr [&dT_dx[6]]);
		}

		if(t7Declared)
		{
			addps(xmmreg(t7Register), xmmword_ptr [&dT_dx[7]]);
		}

		movss(xmmreg(tmp), dword_ptr[&RHW]);
		addss(xmmreg(tmp), dword_ptr[&dw_dx]);
		movss(dword_ptr [&RHW], xmmreg(tmp));

		movss(xmmreg(tmp), dword_ptr[&Z]);
		addss(xmmreg(tmp), dword_ptr[&dz_dx]);
		movss(dword_ptr [&Z], xmmreg(tmp));

		freeAllTemp();

		// Save interpolants for next pixel
		for(int i = 0; i < 8; i++)
		{
			if(xmm[i].type == Operand::COLOR_REGISTER ||
			   xmm[i].type == Operand::INPUT_TEXTURE_COORDINATE_REGISTER)
			{
				spill(i);
			}
		}
	}

	const OperandXMMREG PS20Assembler::xmmreg(const Operand &op, int next, bool raw)
	{
		Operand reg = op;
		reg.index += next;

		checkDcl(op);

		if(reg.type == Operand::CONSTANT_FLOAT_REGISTER)
		{
			throw Error("Cannot write to constant register");
		}

		// Test if already allocated
		for(int i = 0; i < 8; i++)
		{
			if(reg == xmm[i])
			{
				return OperandXMMREG((Encoding::Reg)i);
			}
		}

		// Test for unallocated register
		for(i = 0; i < 8; i++)
		{
			if(xmm[i].type == Operand::VOID)
			{
				return allocate(i, reg, raw);
			}
		}

		// Free register, search for last used
		unsigned char life = 0;
		Instruction *iterator = instruction;
		int lastNonVirtual = -1;

		while(iterator)
		{
			for(int i = 0; i < 8; i++)
			{
				if(xmm[i].type == Operand::VIRTUAL_REGISTER ||
				   xmm[i] == iterator->destination ||
				   xmm[i] == iterator->source0 ||
				   xmm[i] == iterator->source1 ||
				   xmm[i] == iterator->source2 ||
				   xmm[i] == iterator->source3)
				{
					life |= 1 << i;

					if(xmm[i].type != Operand::VIRTUAL_REGISTER)
					{
						lastNonVirtual = i;
					}

					if(life == 0xFF)
					{
						if(lastNonVirtual != -1)
						{
							spill(i);
							return allocate(i, reg, raw);
						}
						else
						{
							// Out of registers
							// Shader instruction tried to allocate more than 8 physical registers
							// Use free() to release virtual (tmp) registers
							throw INTERNAL_ERROR;
						}
					}
				}
			}

			iterator = iterator->next;
		}

		// Dead register available, if it's not an interpolant
		for(i = 0; i < 8; i++)
		{
			if(!(life & (1 << i)))
			{
				if(xmm[i].type != Operand::COLOR_REGISTER &&
				   xmm[i].type != Operand::INPUT_TEXTURE_COORDINATE_REGISTER)
				{
					free(i);
					return allocate(i, reg, raw);
				}
			}
		}

		if(lastNonVirtual != -1)
		{
			spill(i);
			return allocate(i, reg, raw);
		}
		else
		{
			// Check once more for spillable registers
			// Needed when instruction == 0 (last)
			for(int i = 0; i < 8; i++)
			{
				if(xmm[i].type != Operand::VIRTUAL_REGISTER)
				{
					spill(i);
					return allocate(i, reg, raw);	
				}
			}

			// Out of registers
			// Shader instruction tried to allocate more than 8 physical registers
			// Use free() to release virtual (tmp) registers
			throw INTERNAL_ERROR;
		}

		throw INTERNAL_ERROR;
	}

	const OperandXMMREG PS20Assembler::xmmraw(const Operand &op, int next)
	{
		return xmmreg(op, next, true);
	}

	const OperandR_M128 PS20Assembler::r_m128(const Operand &op, int next)
	{
		Operand r_m = op;
		r_m.index += next;

		checkDcl(op);

		if(r_m.type == Operand::CONSTANT_FLOAT_REGISTER)
		{
			// TODO: Check index in range
			return (OperandR_M128&)OperandMEM128(&c[r_m.index]);
		}

		// Test if already allocated
		for(int i = 0; i < 8; i++)
		{
			if(r_m == xmm[i])
			{
				return OperandXMMREG((Encoding::Reg)i);
			}
		}

		if(r_m.type == Operand::VIRTUAL_REGISTER)
		{
			return xmmreg(op, next);
		}

		switch(r_m.type)
		{
		case Operand::COLOR_REGISTER:
			return (OperandR_M128&)OperandMEM128(&v[r_m.index]);
			break;
		case Operand::INPUT_TEXTURE_COORDINATE_REGISTER:
			return (OperandR_M128&)OperandMEM128(&t[r_m.index]);
			break;
		case Operand::SAMPLER_REGISTER:
			throw Error("s%d is read-only", r_m.index);
			break;
		case Operand::TEMPORARY_REGISTER:
			return (OperandR_M128&)OperandMEM128(&r[r_m.index]);
			break;
		case Operand::OUTPUT_COLOR_REGISTER:
			return (OperandR_M128&)OperandMEM128(&oC[r_m.index]);
			break;
		case Operand::OUTPUT_DEPTH_REGISTER:
			return (OperandR_M128&)OperandMEM128(&oDepth);
			break;
		default:
			throw INTERNAL_ERROR;
		}
	}

	const OperandXMM32 PS20Assembler::r_m32(const Operand &r_m, int next)
	{
		return (OperandXMM32&)r_m128(r_m, next);
	}

	void PS20Assembler::checkDcl(const Operand &op)
	{
		if(op.type == Operand::COLOR_REGISTER)
		{
			switch(op.index)
			{
			case 0: if(v0Declared) return; break;
			case 1: if(v1Declared) return; break;
			default:
				throw INTERNAL_ERROR;
			}

			throw Error("Use of undeclared register v%d", op.index);
		}
		else if(op.type == Operand::INPUT_TEXTURE_COORDINATE_REGISTER)
		{
			switch(op.index)
			{
			case 0: if(t0Declared) return; break;
			case 1: if(t1Declared) return; break;
			case 2: if(t2Declared) return; break;
			case 3: if(t3Declared) return; break;
			case 4: if(t4Declared) return; break;
			case 5: if(t5Declared) return; break;
			case 6: if(t6Declared) return; break;
			case 7: if(t7Declared) return; break;
			default:
				throw INTERNAL_ERROR;
			}

			throw Error("Use of undeclared register t%d", op.index);	
		}
	}

	const OperandXMMREG PS20Assembler::allocate(int i, const Operand &reg, bool raw)
	{
		if(xmm[i].type != Operand::VOID)
		{
			// Use free() first
			throw INTERNAL_ERROR;
		}

		switch(reg.type)
		{
		case Operand::VIRTUAL_REGISTER:
			annotate("Assign tmp%d to xmm%d", reg.index, i);
			break;
		case Operand::COLOR_REGISTER:
			annotate("Allocate v%d to xmm%d", reg.index, i);
			break;
		case Operand::INPUT_TEXTURE_COORDINATE_REGISTER:
			annotate("Allocate t%d to xmm%d", reg.index, i);
			break;
		case Operand::SAMPLER_REGISTER:
			throw INTERNAL_ERROR;
			break;
		case Operand::TEMPORARY_REGISTER:
			annotate("Allocate r%d to xmm%d", reg.index, i);
			break;
		case Operand::OUTPUT_COLOR_REGISTER:
			annotate("Allocate oC%d to xmm%d", reg.index, i);
			break;
		case Operand::OUTPUT_DEPTH_REGISTER:
			annotate("Allocate oDepth to xmm%d", i);
			break;
		default:
			throw INTERNAL_ERROR;
		}

		if(reg.type != Operand::VIRTUAL_REGISTER && !raw)
		{
			movaps(OperandXMMREG((Encoding::Reg)i), r_m128(reg));
		}
		
		xmm[i] = reg;
		
		return OperandXMMREG((Encoding::Reg)i);
	}

	void PS20Assembler::spill(int i)
	{
		switch(xmm[i].type)
		{
		case Operand::VOID:
			// Already spilled
			break;
		case Operand::VIRTUAL_REGISTER:
			annotate("Free xmm%d (tmp%d)", i, xmm[i].index);
			break;
		case Operand::COLOR_REGISTER:
			annotate("Spill xmm%d to v%d", i, xmm[i].index);
			movaps(OperandMEM128(&v[xmm[i].index]), OperandXMMREG((Encoding::Reg)i));
			break;
		case Operand::CONSTANT_FLOAT_REGISTER:
			// Constant registers need no spilling		
			break;
		case Operand::INPUT_TEXTURE_COORDINATE_REGISTER:
			annotate("Spill xmm%d to t%d", i, xmm[i].index);
			movaps(OperandMEM128(&t[xmm[i].index]), OperandXMMREG((Encoding::Reg)i));
			break;
		case Operand::SAMPLER_REGISTER:
			annotate("Spill xmm%d to s%d", i, xmm[i].index);
			movaps(OperandMEM128(&s[xmm[i].index]), OperandXMMREG((Encoding::Reg)i));
			break;
		case Operand::TEMPORARY_REGISTER:
			annotate("Spill xmm%d to r%d", i, xmm[i].index);
			movaps(OperandMEM128(&r[xmm[i].index]), OperandXMMREG((Encoding::Reg)i));
			break;
		case Operand::OUTPUT_COLOR_REGISTER:
			annotate("Spill xmm%d to oC%d", i, xmm[i].index);
			movaps(OperandMEM128(&oC[xmm[i].index]), OperandXMMREG((Encoding::Reg)i));
			break;
		case Operand::OUTPUT_DEPTH_REGISTER:
			annotate("Spill xmm%d to oDepth", i);
			movaps(OperandMEM128(oDepth), OperandXMMREG((Encoding::Reg)i));
			break;
		default:
			throw INTERNAL_ERROR;
		}

		free(i);
	}

	void PS20Assembler::free(const Operand &tmp)
	{
		for(int i = 0; i < 8; i++)
		{
			if(xmm[i] == tmp)
			{
				free(i);
			}
		}
	}

	void PS20Assembler::free(int i)
	{
		switch(xmm[i].type)
		{
		case Operand::VOID:
			// Already freed
			break;
		case Operand::VIRTUAL_REGISTER:
			annotate("Free xmm%d (tmp%d)", i, xmm[i].index);
			break;
		case Operand::COLOR_REGISTER:
			annotate("Free xmm%d (v%d)", i, xmm[i].index);
			movaps(OperandMEM128(&v[xmm[i].index]), OperandXMMREG((Encoding::Reg)i));
			break;
		case Operand::CONSTANT_FLOAT_REGISTER:
			annotate("Free xmm%d (c%d)", i, xmm[i].index);
			movaps(OperandMEM128(&c[xmm[i].index]), OperandXMMREG((Encoding::Reg)i));
			break;
		case Operand::INPUT_TEXTURE_COORDINATE_REGISTER:
			annotate("Free xmm%d (t%d)", i, xmm[i].index);
			movaps(OperandMEM128(&t[xmm[i].index]), OperandXMMREG((Encoding::Reg)i));
			break;
		case Operand::SAMPLER_REGISTER:
			// Sampler register should not be allocatedd
			throw INTERNAL_ERROR;
			break;
		case Operand::TEMPORARY_REGISTER:
			annotate("Free xmm%d (r%d)", i, xmm[i].index);
			movaps(OperandMEM128(&r[xmm[i].index]), OperandXMMREG((Encoding::Reg)i));
			break;
		case Operand::OUTPUT_COLOR_REGISTER:
			annotate("Free xmm%d (oC%d)", i, xmm[i].index);
			movaps(OperandMEM128(&oC[xmm[i].index]), OperandXMMREG((Encoding::Reg)i));
			break;
		case Operand::OUTPUT_DEPTH_REGISTER:
			annotate("Free xmm%d (oDepth)", i);
			movaps(OperandMEM128(oDepth), OperandXMMREG((Encoding::Reg)i));
			break;
		default:
			throw INTERNAL_ERROR;
		}

		// No questions asked
		xmm[i].type = Operand::VOID;
	}

	void PS20Assembler::freeAllTemp()
	{
		for(int i = 0; i < 8; i++)
		{
			if(xmm[i].type == Operand::VIRTUAL_REGISTER)
			{
				free(xmm[i]);
			}
		}
	}

	void PS20Assembler::PS_2_0()
	{
		return;
	}

	void PS20Assembler::DCL(const Operand &dest)
	{
		if(dest.type == Operand::COLOR_REGISTER)
		{
			switch(dest.index)
			{
			case 0: v0Declared = true; break;
			case 1: v1Declared = true; break;
			default:
				throw INTERNAL_ERROR;
			}
		}
		else if(dest.type == Operand::INPUT_TEXTURE_COORDINATE_REGISTER)
		{
			switch(dest.index)
			{
			case 0: t0Declared = true; break;
			case 1: t1Declared = true; break;
			case 2: t2Declared = true; break;
			case 3: t3Declared = true; break;
			case 4: t4Declared = true; break;
			case 5: t5Declared = true; break;
			case 6: t6Declared = true; break;
			case 7: t7Declared = true; break;
			default:
				throw INTERNAL_ERROR;
			}
		}
		else
		{
			throw Error("Only color and texture registers can be DCL'ed");
		}
	}

	void PS20Assembler::DCL_2D(const Operand &dest)
	{
		if(dest.type != Operand::SAMPLER_REGISTER) throw INTERNAL_ERROR;
		if(dest.index < 0 || dest.index > 15) throw INTERNAL_ERROR;
		if(!sampler[dest.index].texture) throw Error("No texture set for declared sampler s%d", dest.index);
	}

	void PS20Assembler::DCL_CUBE(const Operand &dest)
	{
		throw Error("Cube maps not supported");
	}

	void PS20Assembler::DCL_VOLUME(const Operand &dest)
	{
		throw Error("Volume textures not supported");
	}

	void PS20Assembler::DEF(const Operand &dest, FValue1 fValue1, FValue2 fValue2, FValue3 fValue3, FValue4 fValue4)
	{
		c[dest.index][0] = fValue1.value;
		c[dest.index][1] = fValue2.value;
		c[dest.index][2] = fValue3.value;
		c[dest.index][3] = fValue4.value;
	}

	void PS20Assembler::ABS(const Operand &dst, const Operand &src)
	{
		static const int4 SIGN_MASK = {0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF};

		NEG_SWIZZLE(tmp, src);

		andps(xmmreg(tmp), xmmword_ptr [SIGN_MASK]);

		SAT_MASK(dst, tmp);
	}

	void PS20Assembler::ADD(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		NEG_SWIZZLE(tmp0, src0);
		NEG_SWIZZLE(tmp1, src1);

		addps(xmmreg(tmp0), r_m128(tmp1));

		SAT_MASK(dst, tmp0);
	}

	void PS20Assembler::CMP(const Operand &dst, const Operand &src0, const Operand &src1, const Operand &src2)
	{
		static const int4 ZERO = {0x00000000, 0x00000000, 0x00000000, 0x00000000};

		NEG_SWIZZLE(tmp0, src0);
		NEG_SWIZZLE(tmp1, src1);
		NEG_SWIZZLE(tmp2, src2);
		
		cmpltps(xmmreg(tmp0), xmmword_ptr [ZERO]);
		andps(xmmreg(tmp2), r_m128(tmp0));
		andnps(xmmreg(tmp0), r_m128(tmp1));
		orps(xmmreg(tmp2), r_m128(tmp0));

		SAT_MASK(dst, tmp2);
	}

	void PS20Assembler::CRS(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		// dest.x = src0.y * src1.z - src0.z * src1.y;
		// dest.y = src0.z * src1.x - src0.x * src1.z;
		// dest.z = src0.x * src1.y - src0.y * src1.x;

		NEG_SWIZZLE(tmp0, src0);
		NEG_SWIZZLE(tmp1, src1);

		movaps(xmmreg(tmp3), r_m128(tmp0));
		movaps(xmmreg(tmp2), r_m128(tmp1));
		shufps(xmmreg(tmp3), r_m128(tmp0), 0xD2);
		shufps(xmmreg(tmp2), r_m128(tmp1), 0xC9);
		mulps(xmmreg(tmp3), r_m128(tmp2));
		movaps(xmmreg(tmp2), r_m128(tmp1));
		shufps(xmmreg(tmp2), r_m128(tmp1), 0xD2);
		movaps(xmmreg(tmp1), r_m128(tmp0));
		shufps(xmmreg(tmp1), r_m128(tmp0), 0xD9);
		mulps(xmmreg(tmp1), r_m128(tmp2));
		subps(xmmreg(tmp1), r_m128(tmp3));

		SAT_MASK(dst, tmp1);
	}

	void PS20Assembler::DP2ADD(const Operand &dst, const Operand &src0, const Operand &src1, const Operand &src2)
	{
		// dest = src0.r * src1.r + src0.g * src1.g + src2.selected_component

		NEG_SWIZZLE(tmp0, src0);
		NEG_SWIZZLE(tmp1, src1);
		
		mulps(xmmreg(tmp0), r_m128(tmp1));
		movlps(xmmreg(tmp1), xmmreg(tmp0));
		shufps(xmmreg(tmp1), r_m128(tmp1), 0x01);
		addss(xmmreg(tmp0), r_m32(tmp1));
		shufps(xmmreg(tmp0), r_m128(tmp0), 0x55);

		SAT_MOV_X(dst, tmp0);
	}

	void PS20Assembler::DP3(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		NEG_SWIZZLE(tmp0, src0);
		NEG_SWIZZLE(tmp1, src1);

		mulps(xmmreg(tmp0), r_m128(tmp1));
		movhlps(xmmreg(tmp1), xmmreg(tmp0));
		addss(xmmreg(tmp1), r_m32(tmp0));
		shufps(xmmreg(tmp0), r_m128(tmp0), 0x01);
		addss(xmmreg(tmp0), r_m32(tmp1));

		SAT_MOV_X(dst, tmp0);
	}

	void PS20Assembler::DP4(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		NEG_SWIZZLE(tmp0, src0);
		NEG_SWIZZLE(tmp1, src1);

		mulps(xmmreg(tmp0), r_m128(tmp1));
		movhlps(xmmreg(tmp1), xmmreg(tmp0));
		addps(xmmreg(tmp0), r_m128(tmp1));
		movss(xmmreg(tmp1), r_m32(tmp0));
		shufps(xmmreg(tmp0), r_m128(tmp0), 0x01);
		addss(xmmreg(tmp1), r_m32(tmp0));

		SAT_MOV_X(dst, tmp1);
	}

	void PS20Assembler::EXP(const Operand &dst, const Operand &src)
	{
		static const float A = -2.91421356e+0f;

		static float TEMP;

		NEG_SWIZZLE(tmp0, src);

		movss(xmmreg(tmp1), r_m32(tmp0));
		cvtss2si(eax, r_m32(tmp1));
		cvtsi2ss(xmmreg(tmp1), eax);
		subss(xmmreg(tmp0), r_m32(tmp1));
		movss(xmmreg(tmp1), dword_ptr [&A]);
		subss(xmmreg(tmp1), r_m32(tmp0));
		addss(xmmreg(tmp0), dword_ptr [&A]);
		rcpss(xmmreg(tmp0), r_m32(tmp0));
		mulss(xmmreg(tmp0), r_m32(tmp1));
		add(eax, 127);
		shl(eax, 23);
		mov(dword_ptr [&TEMP], eax);
		mulss(xmmreg(tmp0), dword_ptr [&TEMP]);
		shufps(xmmreg(tmp0), xmmreg(tmp0), 0x00);

		SAT_MASK(dst, tmp0);
	}

	void PS20Assembler::FRC(const Operand &dst, const Operand &src)
	{
		static const float4 HALF = {0.5f, 0.5f, 0.5f, 0.5f};
		static const int4 MASK = {0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF};

		NEG_SWIZZLE(tmp0, src);

		andps(xmmreg(tmp0), xmmword_ptr [MASK]);
		subps(xmmreg(tmp0), xmmword_ptr [HALF]);
		cvtps2pi(mm0, xmmreg(tmp0));
		movhlps(xmmreg(tmp1), xmmreg(tmp0));
		cvtps2pi(mm1, xmmreg(tmp1));
		cvtpi2ps(xmmreg(tmp1), mm1);
		movlhps(xmmreg(tmp1), xmmreg(tmp1));
		cvtpi2ps(xmmreg(tmp1), mm0);
		addps(xmmreg(tmp0), xmmword_ptr [HALF]);
		subps(xmmreg(tmp0), r_m128(tmp1));

		SAT_MASK(dst, tmp0);
	}

	void PS20Assembler::LOG(const Operand &dst, const Operand &src)
	{
		static const float A =  3.42234550e0f;
		static const float B =  1.42234550e0f;

		static float TEMP1;
		static float TEMP2;

		NEG_SWIZZLE(tmp0, src);

		movss(dword_ptr [&TEMP1], xmmreg(tmp0));
		mov(eax, dword_ptr [&TEMP1]);
		and(eax, 0x007FFFFF);
		or(eax, 0x3F800000);
		mov(dword_ptr [&TEMP2], eax);
		movss(xmmreg(tmp0), dword_ptr [&TEMP2]);
		movss(xmmreg(tmp1), r_m32(tmp0));
		mulss(xmmreg(tmp0), dword_ptr [&A]);
		subss(xmmreg(tmp0), dword_ptr [&A]);
		addss(xmmreg(tmp1), dword_ptr [&B]);
		rcpss(xmmreg(tmp1), r_m32(tmp1));
		mulss(xmmreg(tmp0), r_m32(tmp1));
		mov(eax, dword_ptr [&TEMP1]);
		and(eax, 0x7F800000);
		shr(eax, 23);
		sub(eax, 127);
		cvtsi2ss(xmmreg(tmp1), eax);
		addss(xmmreg(tmp0), r_m32(tmp1));
		shufps(xmmreg(tmp0), xmmreg(tmp0), 0x00);

		SAT_MASK(dst, tmp0);
	}

	void PS20Assembler::LRP(const Operand &dst, const Operand &src0, const Operand &src1, const Operand &src2)
	{
		// dest = src2 + src0 * (src1 - src2)

		NEG_SWIZZLE(tmp0, src0);
		NEG_SWIZZLE(tmp1, src1);
		NEG_SWIZZLE(tmp2, src2);

		subps(xmmreg(tmp1), r_m128(tmp2));
		mulps(xmmreg(tmp1), r_m128(tmp0));
		addps(xmmreg(tmp1), r_m128(tmp2));

		SAT_MASK(dst, tmp1);
	}

	void PS20Assembler::M3X2(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		movaps(xmmreg(tmp0), r_m128(src1, 0));
		movaps(xmmreg(tmp1), r_m128(src1, 1));
		movaps(xmmreg(tmp2), r_m128(tmp0));
		movaps(xmmreg(tmp3), r_m128(tmp1)); 

		unpckhps(xmmreg(tmp0), r_m128(tmp0));
		unpckhps(xmmreg(tmp1), r_m128(tmp1));
		movaps(xmmreg(tmp4), r_m128(tmp0));
		unpcklps(xmmreg(tmp4), r_m128(tmp1));

		unpcklps(xmmreg(tmp2), r_m128(tmp2));
		unpcklps(xmmreg(tmp3), r_m128(tmp3));
		movaps(xmmreg(tmp5), r_m128(tmp2));
		unpckhps(xmmreg(tmp2), r_m128(tmp3));
		unpcklps(xmmreg(tmp5), r_m128(tmp3));

		NEG_SWIZZLE(tmp1, src0);

		movaps(xmmreg(tmp6), r_m128(tmp1));
		movaps(xmmreg(tmp7), r_m128(tmp1));
		shufps(xmmreg(tmp1), r_m128(tmp1), 0x00);
		shufps(xmmreg(tmp6), r_m128(tmp6), 0x55);
		shufps(xmmreg(tmp7), r_m128(tmp7), 0xAA);

		mulps(xmmreg(tmp1), r_m128(tmp5));
		mulps(xmmreg(tmp6), r_m128(tmp2));
		mulps(xmmreg(tmp7), r_m128(tmp4));

		addps(xmmreg(tmp1), r_m128(tmp6));
		addps(xmmreg(tmp1), r_m128(tmp7));

		SAT_MOV_XY(dst, tmp1);
	}

	void PS20Assembler::M3X3(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		movaps(xmmreg(tmp0), r_m128(src1, 0));
		movaps(xmmreg(tmp1), r_m128(src1, 1));
		movaps(xmmreg(tmp2), r_m128(tmp0));
		movaps(xmmreg(tmp3), r_m128(tmp1)); 

		unpckhps(xmmreg(tmp0), r_m128(src1, 2));
		unpckhps(xmmreg(tmp1), r_m128(tmp1));
		movaps(xmmreg(tmp4), r_m128(tmp0));
		unpcklps(xmmreg(tmp4), r_m128(tmp1));

		unpcklps(xmmreg(tmp2), r_m128(src1, 2));
		unpcklps(xmmreg(tmp3), r_m128(tmp3));
		movaps(xmmreg(tmp5), r_m128(tmp2));
		unpckhps(xmmreg(tmp2), r_m128(tmp3));
		unpcklps(xmmreg(tmp5), r_m128(tmp3));

		NEG_SWIZZLE(tmp1, src0);

		movaps(xmmreg(tmp6), r_m128(tmp1));
		movaps(xmmreg(tmp7), r_m128(tmp1));
		shufps(xmmreg(tmp1), r_m128(tmp1), 0x00);
		shufps(xmmreg(tmp6), r_m128(tmp6), 0x55);
		shufps(xmmreg(tmp7), r_m128(tmp7), 0xAA);

		mulps(xmmreg(tmp1), r_m128(tmp5));
		mulps(xmmreg(tmp6), r_m128(tmp2));
		mulps(xmmreg(tmp7), r_m128(tmp4));

		addps(xmmreg(tmp1), r_m128(tmp6));
		addps(xmmreg(tmp1), r_m128(tmp7));

		SAT_MOV_XYZ(dst, tmp1);
	}

	void PS20Assembler::M3X4(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		movaps(xmmreg(tmp0), r_m128(src1, 0));
		movaps(xmmreg(tmp1), r_m128(src1, 1));
		movaps(xmmreg(tmp2), r_m128(tmp0));
		movaps(xmmreg(tmp3), r_m128(tmp1)); 

		unpckhps(xmmreg(tmp0), r_m128(src1, 2));
		unpckhps(xmmreg(tmp1), r_m128(src1, 3));
		movaps(xmmreg(tmp4), r_m128(tmp0));
		unpcklps(xmmreg(tmp4), r_m128(tmp1));

		unpcklps(xmmreg(tmp2), r_m128(src1, 2));
		unpcklps(xmmreg(tmp3), r_m128(src1, 3));
		movaps(xmmreg(tmp5), r_m128(tmp2));
		unpckhps(xmmreg(tmp2), r_m128(tmp3));
		unpcklps(xmmreg(tmp5), r_m128(tmp3));

		NEG_SWIZZLE(tmp1, src0);

		movaps(xmmreg(tmp6), r_m128(tmp1));
		movaps(xmmreg(tmp7), r_m128(tmp1));
		shufps(xmmreg(tmp1), r_m128(tmp1), 0x00);
		shufps(xmmreg(tmp6), r_m128(tmp6), 0x55);
		shufps(xmmreg(tmp7), r_m128(tmp7), 0xAA);

		mulps(xmmreg(tmp1), r_m128(tmp5));
		mulps(xmmreg(tmp6), r_m128(tmp2));
		mulps(xmmreg(tmp7), r_m128(tmp4));

		addps(xmmreg(tmp1), r_m128(tmp6));
		addps(xmmreg(tmp1), r_m128(tmp7));

		SAT_MOV_XYZW(dst, tmp1);
	}

	void PS20Assembler::M4X3(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		movaps(xmmreg(tmp0), r_m128(src1, 0));
		movaps(xmmreg(tmp1), r_m128(src1, 1));
		movaps(xmmreg(tmp2), r_m128(tmp0));
		movaps(xmmreg(tmp3), r_m128(tmp1)); 

		unpckhps(xmmreg(tmp0), r_m128(src1, 2));
		unpckhps(xmmreg(tmp1), r_m128(tmp1));
		movaps(xmmreg(tmp4), r_m128(tmp0)); 
		unpckhps(xmmreg(tmp0), r_m128(tmp1));
		unpcklps(xmmreg(tmp4), r_m128(tmp1));

		unpcklps(xmmreg(tmp2), r_m128(src1, 2));
		unpcklps(xmmreg(tmp3), r_m128(tmp3));
		movaps(xmmreg(tmp5), r_m128(tmp2));
		unpckhps(xmmreg(tmp2), r_m128(tmp3));
		unpcklps(xmmreg(tmp5), r_m128(tmp3));

		NEG_SWIZZLE(tmp1, src0);

		movaps(xmmreg(tmp6), r_m128(tmp1));
		movaps(xmmreg(tmp7), r_m128(tmp1));
		movaps(xmmreg(tmp3), r_m128(tmp1));
		shufps(xmmreg(tmp1), r_m128(tmp1), 0x00);
		shufps(xmmreg(tmp6), r_m128(tmp6), 0x55);
		shufps(xmmreg(tmp7), r_m128(tmp7), 0xAA);
		shufps(xmmreg(tmp3), r_m128(tmp3), 0xFF);

		mulps(xmmreg(tmp1), r_m128(tmp5));
		mulps(xmmreg(tmp6), r_m128(tmp2));
		mulps(xmmreg(tmp7), r_m128(tmp4));
		mulps(xmmreg(tmp3), r_m128(tmp0));

		addps(xmmreg(tmp1), r_m128(tmp6));
		addps(xmmreg(tmp7), r_m128(tmp3));
		addps(xmmreg(tmp1), r_m128(tmp7));

		SAT_MOV_XYZ(dst, tmp1);
	}

	void PS20Assembler::M4X4(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		movaps(xmmreg(tmp0), r_m128(src1, 0));
		movaps(xmmreg(tmp1), r_m128(src1, 1));
		movaps(xmmreg(tmp2), r_m128(tmp0));
		movaps(xmmreg(tmp3), r_m128(tmp1)); 

		unpckhps(xmmreg(tmp0), r_m128(src1, 2));
		unpckhps(xmmreg(tmp1), r_m128(src1, 3));
		movaps(xmmreg(tmp4), r_m128(tmp0)); 
		unpckhps(xmmreg(tmp0), r_m128(tmp1));
		unpcklps(xmmreg(tmp4), r_m128(tmp1));

		unpcklps(xmmreg(tmp2), r_m128(src1, 2));
		unpcklps(xmmreg(tmp3), r_m128(src1, 3));
		movaps(xmmreg(tmp5), r_m128(tmp2));
		unpckhps(xmmreg(tmp2), r_m128(tmp3));
		unpcklps(xmmreg(tmp5), r_m128(tmp3));

		NEG_SWIZZLE(tmp1, src0);

		movaps(xmmreg(tmp6), r_m128(tmp1));
		movaps(xmmreg(tmp7), r_m128(tmp1));
		movaps(xmmreg(tmp3), r_m128(tmp1));
		shufps(xmmreg(tmp1), r_m128(tmp1), 0x00);
		shufps(xmmreg(tmp6), r_m128(tmp6), 0x55);
		shufps(xmmreg(tmp7), r_m128(tmp7), 0xAA);
		shufps(xmmreg(tmp3), r_m128(tmp3), 0xFF);

		mulps(xmmreg(tmp1), r_m128(tmp5));
		mulps(xmmreg(tmp6), r_m128(tmp2));
		mulps(xmmreg(tmp7), r_m128(tmp4));
		mulps(xmmreg(tmp3), r_m128(tmp0));

		addps(xmmreg(tmp1), r_m128(tmp6));
		addps(xmmreg(tmp7), r_m128(tmp3));
		addps(xmmreg(tmp1), r_m128(tmp7));

		SAT_MOV_XYZW(dst, tmp1);
	}

	void PS20Assembler::MAD(const Operand &dst, const Operand &src0, const Operand &src1, const Operand &src2)
	{
		NEG_SWIZZLE(tmp0, src0);
		NEG_SWIZZLE(tmp1, src1);
		NEG_SWIZZLE(tmp2, src2);

		mulps(xmmreg(tmp0), r_m128(tmp1));
		addps(xmmreg(tmp0), r_m128(tmp2));

		SAT_MASK(dst, tmp0);
	}

	void PS20Assembler::MAX(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		NEG_SWIZZLE(tmp0, src0);
		NEG_SWIZZLE(tmp1, src1);

		maxps(xmmreg(tmp0), r_m128(tmp1));

		SAT_MASK(dst, tmp0);
	}

	void PS20Assembler::MIN(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		NEG_SWIZZLE(tmp0, src0);
		NEG_SWIZZLE(tmp1, src1);

		minps(xmmreg(tmp0), r_m128(tmp1));

		SAT_MASK(dst, tmp0);
	}

	void PS20Assembler::MOV(const Operand &dst, const Operand &src)
	{
		NEG_SWIZZLE(tmp, src);
		SAT_MASK(dst, tmp);
	}

	void PS20Assembler::MUL(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		NEG_SWIZZLE(tmp0, src0);
		NEG_SWIZZLE(tmp1, src1);

		mulps(xmmreg(tmp0), r_m128(tmp1));

		SAT_MASK(dst, tmp0);
	}

	void PS20Assembler::NOP()
	{
		nop();
	}

	void PS20Assembler::NRM(const Operand &dst, const Operand &src)
	{
		NEG_SWIZZLE(tmp0, src);
		NEG_SWIZZLE(tmp1, dst);

		movaps(xmmreg(tmp1), r_m128(tmp0));
		mulps(xmmreg(tmp1), r_m128(tmp0));
		movhlps(xmmreg(tmp0), xmmreg(tmp1));
		addss(xmmreg(tmp0), r_m32(tmp1));
		shufps(xmmreg(tmp1), r_m128(tmp1), 0x01);
		addss(xmmreg(tmp1), r_m32(tmp0));
		rsqrtss(xmmreg(tmp1), r_m32(tmp1));
		shufps(xmmreg(tmp1), r_m128(tmp1), 0x00);
		mulps(xmmreg(tmp1), r_m128(tmp0));

		SAT_MASK(dst, tmp1);
	}

	void PS20Assembler::POW(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		LOG(tmp0, src0);
		NEG_SWIZZLE(tmp1, src1);

		mulss(xmmreg(tmp0), r_m32(src1));

		EXP(dst, tmp0);
	}

	void PS20Assembler::RCP(const Operand &dst, const Operand &src)
	{
		NEG_SWIZZLE(tmp0, src);

		if(instruction->modifier == Instruction::_PP)
		{
			rcpss(xmmreg(tmp1), r_m32(tmp0));
		}
		else
		{
			movss(xmmreg(tmp2), r_m32(tmp0));
			rcpss(xmmreg(tmp1), r_m32(tmp0));
			mulss(xmmreg(tmp2), r_m32(tmp1));
			mulss(xmmreg(tmp2), r_m32(tmp1));
			addss(xmmreg(tmp1), r_m32(tmp1));
			subss(xmmreg(tmp1), r_m32(tmp2));
		}

		shufps(xmmreg(tmp1), xmmreg(tmp1), 0x00);

		SAT_MASK(dst, tmp1);
	}

	void PS20Assembler::RSQ(const Operand &dst, const Operand &src)
	{
		NEG_SWIZZLE(tmp0, src);

		if(instruction->modifier == Instruction::_PP)
		{
			rsqrtss(xmmreg(tmp1), r_m32(tmp0));
		}
		else
		{
			static const float THREE = 3.0f;
			static const float HALF = 0.5f;

			rsqrtss(xmmreg(tmp0), r_m32(tmp0));
			movss(xmmreg(tmp1), r_m32(tmp0));
			mulss(xmmreg(tmp0), r_m32(tmp0));
			mulss(xmmreg(tmp0), r_m32(tmp0));
			movss(xmmreg(tmp1), dword_ptr [&THREE]);
			subss(xmmreg(tmp1), r_m32(tmp0));
			mulss(xmmreg(tmp1), r_m32(tmp1));
			mulss(xmmreg(tmp1), dword_ptr [&HALF]);
		}

		shufps(xmmreg(tmp1), xmmreg(tmp1), 0x00);

		SAT_MASK(dst, tmp1);
	}

	void PS20Assembler::SINCOS(const Operand &dst, const Operand &src0, const Operand &src1, const Operand &src2)
	{
		static const float C = -4.96818924e-1f;
		static const float B =  3.95277743e-2f;
		static const float A = -9.84989568e-4f;

		static const float ONE = 1.0f;

		NEG_SWIZZLE(tmp0, src0);

		mulss(xmmreg(tmp0), r_m32(tmp0));
		movss(xmmreg(tmp1), r_m32(tmp0));
		mulss(xmmreg(tmp0), dword_ptr [&C]);
		mulss(xmmreg(tmp1), r_m32(tmp1));
		movss(xmmreg(tmp2), r_m32(tmp1));
		mulss(xmmreg(tmp1), dword_ptr [&B]);
		addss(xmmreg(tmp0), r_m32(tmp1));
		mulss(xmmreg(tmp2), r_m32(tmp0));
		mulss(xmmreg(tmp2), dword_ptr [&A]);
		addss(xmmreg(tmp0), r_m32(tmp2));
		addss(xmmreg(tmp0), dword_ptr [&ONE]);
		movss(xmmreg(tmp1), r_m32(tmp0));
		mulss(xmmreg(tmp1), r_m32(tmp1));
		movss(xmmreg(tmp2), dword_ptr [&ONE]);
		subss(xmmreg(tmp2), r_m32(tmp1));
		rsqrtss(xmmreg(tmp2), r_m32(tmp2));
		rcpss(xmmreg(tmp2), r_m32(tmp2));
		movlhps(xmmreg(tmp0), xmmreg(tmp2));
		shufps(xmmreg(tmp0), r_m128(tmp0), 0x08);

		SAT_MOV_XY(dst, tmp0);
	}

	void PS20Assembler::SUB(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		movaps(xmmreg(dst), r_m128(src0));
		subps(xmmreg(dst), r_m128(src1));
	}

	void PS20Assembler::TEXKILL(const Operand &src)
	{
		static const float4 XYZ = {0, 0, 0, -FLT_MAX};

		movaps(xmmreg(tmp), r_m128(src));

		cmpltps(xmmreg(tmp), xmmword_ptr [&XYZ]);
		movmskps(eax, xmmreg(tmp));
		test(eax, eax);
		jnz("texkill");
	}

	void PS20Assembler::TEXLD(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		const int mipmapOffset = (int)&((Texture*)0)->mipmap;
		const int uFracOffset = (int)&((Texture*)0)->uFrac;
		const int vFracOffset = (int)&((Texture*)0)->vFrac;
		const int uIntOffset = (int)&((Texture*)0)->uInt;
		const int vIntOffset = (int)&((Texture*)0)->vInt;
		const int uHalfOffset = (int)&((Texture*)0)->uHalf;
		const int vHalfOffset = (int)&((Texture*)0)->vHalf;

		movss(xmmreg(tmp0), dword_ptr [&RHW]);
		rcpss(xmmreg(tmp1), xmmreg(tmp0));
		mulss(xmmreg(tmp0), xmmreg(tmp1));
		mulss(xmmreg(tmp0), xmmreg(tmp1));
		addss(xmmreg(tmp1), xmmreg(tmp1));
		subss(xmmreg(tmp1), xmmreg(tmp0));
		shufps(xmmreg(tmp1), xmmreg(tmp1), 0x00);

		movaps(xmmreg(tmp2), r_m128(src0));
		mulps(xmmreg(tmp2), r_m128(tmp1));

		if(sampler[src1.index].addressingMode == Sampler::ADDRESSING_CLAMP)
		{
			static const float4 clampZero = {0, 0, FLT_MIN, FLT_MIN};
			static const float4 clampOne = {1, 1, FLT_MAX, FLT_MAX};

			maxps(xmmreg(tmp2), xmmword_ptr [&clampZero]);
			minps(xmmreg(tmp2), xmmword_ptr [&clampOne]);
		}

		static const float4 scale = {1 << 16, 1 << 16, 0, 1 << 16};

		mulps(xmmreg(tmp2), xmmword_ptr [&scale]);
		cvtps2pi(mm0, xmmreg(tmp2));

		if(sampler[src1.index].addressingMode == Sampler::ADDRESSING_MIRROR)
		{
			movq(mm1, mm0);
			pshufw(mm2, mm0, 0xDD);
			pslld(mm1, 15);
			pslld(mm2, 15);
			psrad(mm1, 31);
			psrad(mm2, 31);
			punpckldq(mm1, mm2);
			pxor(mm0, mm1);
		}

		// Mipmap LOD
		static const float clamp = 65535.0f;

		shufps(xmmreg(tmp2), xmmreg(tmp2), 0xFF);
		mulss(xmmreg(tmp2), xmmreg(tmp1));
		minss(xmmreg(tmp2), dword_ptr [&clamp]);
		cvtss2si(ebx, xmmreg(tmp2));
		bsr(ebx, ebx);

		mov(ebp, dword_ptr [&sampler[src1.index].texture]);
		mov(esi, dword_ptr [ebp+4*ebx+mipmapOffset]);

		if(sampler[src1.index].textureFilter == Sampler::FILTER_LINEAR)
		{
			static const qword _F_F = 0x0000FFFF0000FFFF;
			static const qword __FF = 0x00000000FFFFFFFF;

			pshufw(mm1, mm0, 0xAA);
			pshufw(mm0, mm0, 0x00);

			paddw(mm1, qword_ptr [ebp+8*ebx+vHalfOffset]);
			paddw(mm0, qword_ptr [ebp+8*ebx+uHalfOffset]);

			movq(mm3, mm1);
			movq(mm2, mm0);
			movq(mm4, mm0);
			psrlw(mm3, qword_ptr [ebp+8*ebx+vFracOffset]);
			punpcklwd(mm2, mm3);
			punpckhwd(mm4, mm3);
			psrld(mm2, qword_ptr [ebp+8*ebx+uFracOffset]);
			psrld(mm4, qword_ptr [ebp+8*ebx+uFracOffset]);

			psllw(mm0, qword_ptr [ebp+8*ebx+uIntOffset]);
			psllw(mm1, qword_ptr [ebp+8*ebx+vIntOffset]);
			pxor(mm0, qword_ptr [&_F_F]);
			pxor(mm1, qword_ptr [&__FF]);
			pmulhuw(mm0, mm1);

			movd(eax, mm2);
			punpcklbw(mm3, qword_ptr [esi+eax*4]);
			psrlq(mm2, 32);
			movd(eax, mm2);
			punpcklbw(mm2, qword_ptr [esi+eax*4]);

			movd(eax, mm4);
			punpcklbw(mm5, qword_ptr [esi+eax*4]);
			psrlq(mm4, 32);
			movd(eax, mm4);
			punpcklbw(mm4, qword_ptr [esi+eax*4]);

			pshufw(mm1, mm0, 0x00);
			pmulhuw(mm3, mm1);
			pshufw(mm1, mm0, 0x55);
			pmulhuw(mm2, mm1);
			pshufw(mm1, mm0, 0xAA);
			pmulhuw(mm5, mm1);
			pshufw(mm1, mm0, 0xFF);
			pmulhuw(mm4, mm1);

			paddusw(mm3, mm2);
			paddusw(mm5, mm4);
			paddusw(mm5, mm3);
		}
		else   // FILTER_POINT
		{
			pshufw(mm1, mm0, 0xAA);
			psrlw(mm1, qword_ptr [ebp+8*ebx+vFracOffset]);
			punpcklwd(mm0, mm1);
			psrld(mm0, qword_ptr [ebp+8*ebx+uFracOffset]);

			movd(eax, mm0);
			punpcklbw(mm5, qword_ptr [esi+eax*4]);
		}

		psrlw(mm5, 8);
		punpckhwd(mm1, mm5);
		punpcklwd(mm5, mm5);

		static const float4 unscale = {1.0f / 0x00FF00FF, 1.0f / 0x00FF00FF, 1.0f / 0x00FF00FF, 1.0f / 0x00FF00FF};

		cvtpi2ps(xmmreg(dst), mm1);
		movlhps(xmmreg(dst), xmmreg(dst));
		cvtpi2ps(xmmreg(dst), mm5);
		mulps(xmmreg(dst), xmmword_ptr [&unscale]);
	}

	void PS20Assembler::TEXLDB(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		throw Error("TEXLDB not supported");
	}

	void PS20Assembler::TEXLDP(const Operand &dst, const Operand &src0, const Operand &src1)
	{
		throw Error("TEXLDP not supported");
	}

	// Helper macro instructions

	void PS20Assembler::NEG(const Operand &tmp, const Operand &src)
	{
		static const int4 SIGN_MASK = {0x80000000, 0x80000000, 0x80000000, 0x80000000};

		movaps(xmmreg(tmp), r_m128(src));

		if(src.mod == Operand::NEGATE)
		{
			xorps(xmmreg(tmp), xmmword_ptr [SIGN_MASK]);
		}
	}

	void PS20Assembler::SWIZZLE(const Operand &tmp, const Operand &src)
	{
		movaps(xmmreg(tmp), r_m128(src));
		shufps(xmmreg(tmp), xmmreg(tmp), src.swizzle());
	}

	void PS20Assembler::MASK(const Operand &dst, const Operand &tmp)
	{
		if(dst.type == Operand::OUTPUT_COLOR_REGISTER)
		{
			movaps(xmmreg(dst), r_m128(tmp));
			return;
		}

		if(dst.sel == xMask)
		{
			movss(xmmreg(dst), r_m32(tmp));
		}
		else if(dst.sel == xyzwMask)
		{
			movaps(xmmreg(dst), xmmreg(tmp));
		/*
			// Free old dst and make tmp the new dst
			for(int i = 0; i < 8; i++)
			{
				if(xmm[i] == dst)
				{
					free(i);
					break;
				}
			}

			for(i = 0; i < 8; i++)
			{
				if(xmm[i] == tmp)
				{
					free(i);
					allocate(i, dst);
					break;
				}
			}

			// Not allocated to a register
			if(i == 8)
			{
				movaps(xmmreg(dst), r_m128(tmp));
			}
		*/
		}
		else
		{
			static const int4 MASK[] = {{-1,  0,  0,  0},	// x
			                            { 0, -1,  0,  0},	// y
			                            { 0,  0, -1,  0},	// z
		                                { 0,  0,  0, -1},	// w
			                            {-1, -1,  0,  0},	// xy
			                            {-1,  0, -1,  0},	// xz
		                                {-1,  0,  0, -1},	// xw
			                            { 0, -1, -1,  0},	// yz
			                            { 0, -1,  0, -1},	// yw
			                            { 0,  0, -1, -1},	// zw
			                            {-1, -1, -1,  0},	// xyz
			                            {-1, -1,  0, -1},	// xyw
		                                {-1,  0, -1, -1},	// xzw
			                            { 0, -1, -1, -1},	// yzw
			                            {-1, -1, -1, -1}};	// xyzw

			int m = -1;

			if(dst.sel == xMask) m = 0;
			if(dst.sel == yMask) m = 1;
			if(dst.sel == zMask) m = 2;
			if(dst.sel == wMask) m = 3;
			if(dst.sel == xyMask) m = 4;
			if(dst.sel == xzMask) m = 5;
			if(dst.sel == xwMask) m = 6;
			if(dst.sel == yzMask) m = 7;
			if(dst.sel == ywMask) m = 8;
			if(dst.sel == zwMask) m = 9;
			if(dst.sel == xyzMask) m = 10;
			if(dst.sel == xywMask) m = 11;
			if(dst.sel == xzwMask) m = 12;
			if(dst.sel == yzwMask) m = 13;
			if(dst.sel == xyzwMask) m = 14;

			if(m == -1) throw INTERNAL_ERROR;

			subps(xmmreg(tmp), r_m128(dst));
			andps(xmmreg(tmp), xmmword_ptr [&MASK[m]]);
			addps(xmmreg(dst), r_m128(tmp));
		}
	}

	void PS20Assembler::SAT(const Operand &dst, const Operand &tmp)
	{
		if(instruction->modifier == Instruction::_SAT)
		{
			static const float4 ZERO = {0, 0, 0, 0};
			static const float4 ONE = {1, 1, 1, 1};

			movaps(xmmreg(dst), r_m128(tmp));
			maxps(xmmreg(dst), xmmword_ptr [ZERO]);
			minps(xmmreg(dst), xmmword_ptr [ONE]);
		}
	}

	void PS20Assembler::NEG_SWIZZLE(const Operand &tmp, const Operand &src)
	{
		NEG(tmp, src);
		SWIZZLE(tmp, tmp);
	}

	void PS20Assembler::SAT_MASK(const Operand &dst, const Operand &tmp)
	{
		SAT(tmp, tmp);
		MASK(dst, tmp);
	}

	void PS20Assembler::SAT_MOV_X(const Operand &dst, const Operand &tmp)
	{
		Operand dst_x = dst;
		dst_x.sel = xMask;

		SAT(tmp, tmp);
		MASK(dst_x, tmp);
	}

	void PS20Assembler::SAT_MOV_XY(const Operand &dst, const Operand &tmp)
	{
		Operand dst_xy = dst;
		dst_xy.sel = xyMask;

		SAT(tmp, tmp);
		MASK(dst_xy, tmp);
	}

	void PS20Assembler::SAT_MOV_XYZ(const Operand &dst, const Operand &tmp)
	{
		Operand dst_xyz = dst;
		dst_xyz.sel = xyzMask;

		SAT(tmp, tmp);
		MASK(dst_xyz, tmp);
	}

	void PS20Assembler::SAT_MOV_XYZW(const Operand &dst, const Operand &tmp)
	{
		Operand dst_xyzw = dst;
		dst_xyzw.sel = xyzwMask;

		SAT(tmp, tmp);
		MASK(dst_xyzw, tmp);
	}

	void PS20Assembler::movaps(OperandXMMREG tmp1, OperandXMMREG xmm2)
	{
		if(tmp1.reg != xmm2.reg)
		{
			Assembler::movaps(tmp1, xmm2);
		}
	}

	void PS20Assembler::movaps(OperandXMMREG xmm, OperandMEM128 m128)
	{
		Assembler::movaps(xmm, m128);
	}

	void PS20Assembler::movaps(OperandXMMREG xmm, OperandR_M128 r_m128)
	{
		if(r_m128.type != SoftWire::Operand::XMMREG || xmm.reg != r_m128.reg)
		{
			Assembler::movaps(xmm, r_m128);
		}
	}

	void PS20Assembler::movaps(OperandMEM128 m128, OperandXMMREG xmm)
	{
		Assembler::movaps(m128, xmm);
	}

	void PS20Assembler::movaps(OperandR_M128 r_m128, OperandXMMREG xmm)
	{
		if(r_m128.type != SoftWire::Operand::XMMREG || r_m128.reg != xmm.reg)
		{
			Assembler::movaps(r_m128, xmm);
		}
	}

	void PS20Assembler::movups(OperandXMMREG tmp1, OperandXMMREG xmm2)
	{
		Assembler::movups(tmp1, xmm2);
	}

	void PS20Assembler::movups(OperandXMMREG xmm, OperandMEM128 m128)
	{
		Assembler::movups(xmm, m128);
	}

	void PS20Assembler::movups(OperandXMMREG xmm, OperandR_M128 r_m128)
	{
		Assembler::movups(xmm, r_m128);
	}

	void PS20Assembler::movups(OperandMEM128 m128, OperandXMMREG xmm)
	{
		Assembler::movups(m128, xmm);
	}

	void PS20Assembler::movups(OperandR_M128 r_m128, OperandXMMREG xmm)
	{
		Assembler::movups(r_m128, xmm);
	}

	void PS20Assembler::movss(OperandXMMREG tmp1, OperandXMMREG xmm2)
	{
		Assembler::movss(tmp1, xmm2);
	}

	void PS20Assembler::movss(OperandXMMREG xmm, OperandMEM32 m32)
	{
		Assembler::movss(xmm, m32);
	}

	void PS20Assembler::movss(OperandXMMREG xmm, OperandXMM32 r_m32)
	{
		Assembler::movss(xmm, r_m32);
	}

	void PS20Assembler::movss(OperandMEM32 m32, OperandXMMREG xmm)
	{
		Assembler::movss(m32, xmm);
	}

	void PS20Assembler::movss(OperandXMM32 r_m32, OperandXMMREG xmm)
	{
		Assembler::movss(r_m32, xmm);
	}

	void PS20Assembler::movhlps(OperandXMMREG tmp1, OperandXMMREG xmm2)
	{
		Assembler::movhlps(tmp1, xmm2);
	}

	void PS20Assembler::movlhps(OperandXMMREG tmp1, OperandXMMREG xmm2)
	{
		Assembler::movlhps(tmp1, xmm2);
	}

	void PS20Assembler::shufps(OperandXMMREG tmp1, OperandXMMREG xmm2, unsigned char imm8)
	{
		if(tmp1.reg != xmm2.reg || imm8 != 0xE4)
		{
			Assembler::shufps(tmp1, xmm2, imm8);
		}
	}

	void PS20Assembler::shufps(OperandXMMREG xmm, OperandMEM128 m128, unsigned char imm8)
	{
		Assembler::shufps(xmm, m128, imm8);
	}

	void PS20Assembler::shufps(OperandXMMREG xmm, OperandR_M128 r_m128, unsigned char imm8)
	{
		if(r_m128.type != SoftWire::Operand::XMMREG || xmm.reg != r_m128.reg || imm8 != 0xE4)
		{
			Assembler::shufps(xmm, r_m128, imm8);
		}
	}

	void PS20Assembler::pshufw(OperandMMREG mm1, OperandMMREG mm2, unsigned char imm8)
	{
		Assembler::pshufw(mm1, mm2, imm8);
	}

	void PS20Assembler::pshufw(OperandMMREG mm, OperandR_M64 r_m64, unsigned char imm8)
	{
		Assembler::pshufw(mm, r_m64, imm8);
	}

	void PS20Assembler::pshufw(OperandMMREG mm, OperandMEM64 mem64, unsigned char imm8)
	{
		Assembler::pshufw(mm, mem64, imm8);
	}
}
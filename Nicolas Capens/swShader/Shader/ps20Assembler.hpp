#ifndef swShader_PS20Assembler_hpp
#define swShader_PS20Assembler_hpp

#include "Assembler.hpp"
#include "X86Shader.hpp"
#include "Context.hpp"

#include "Instruction.hpp"
#include "Operand.hpp"
#include "Types.hpp"

namespace swShader
{
	using namespace SoftWire;

	class PS20Assembler : protected Assembler, protected Context, public X86Shader
	{
	public:
		PS20Assembler();

		virtual ~PS20Assembler();

		void (*shader())();

		static void setConstant(int index, const float value[4]);

	protected:
		void setMnemonic(Instruction::Mnemonic mnemonic);
		void setModifier(Instruction::Modifier modifier);
		void setDestination(const Operand &operand);
		void setSource0(const Operand &operand);
		void setSource1(const Operand &operand);
		void setSource2(const Operand &operand);
		void setSource3(const Operand &operand);

		void newInstruction();

		void encode();

		// Shader registers
		static float4 r[12];
		static float4 t[8];
		static float4 s[16];
		static float4 v[2];
		static float4 c[32];
		static float4 oC[4];
		static float4 oDepth;

		bool v0Declared;
		bool v1Declared;

		bool t0Declared;
		bool t1Declared;
		bool t2Declared;
		bool t3Declared;
		bool t4Declared;
		bool t5Declared;
		bool t6Declared;
		bool t7Declared;

		Instruction *intermediate;
		Instruction *instruction;

	private:
		void setupInterpolants();
		void depthTest();
		void pixel();
		void freeAllTemp();
		void writeOC0();
		void interpolate();

		union
		{
			struct
			{
				Operand tmp;
			};
			struct
			{
				Operand tmp0;
			};
			struct
			{
				Operand temp;
			};
		};
		Operand tmp1;
		Operand tmp2;
		Operand tmp3;
		Operand tmp4;
		Operand tmp5;
		Operand tmp6;
		Operand tmp7;

		Operand::Register xmm[8];   // Register allocation

		// Interpolants not in shader registers
		float RHW;
		float Z;

		const OperandXMMREG xmmreg(const Operand &reg, int next = 0, bool raw = false);
		const OperandXMMREG xmmraw(const Operand &reg, int next = 0);
		const OperandR_M128 r_m128(const Operand &r_m, int next = 0);
		const OperandXMM32 r_m32(const Operand &r_m, int next = 0);

		void checkDcl(const Operand &op);

		const OperandXMMREG allocate(int i, const Operand &reg, bool raw);
		void spill(int i);
		void free(const Operand &tmp);
		void free(int i);

		typedef const Operand &Dst;
		typedef const Operand &Dest;
		typedef const Operand &Src;
		typedef const Operand &Src0;
		typedef const Operand &Src1;
		typedef const Operand &Src2;
		typedef const Operand &FValue1;
		typedef const Operand &FValue2;
		typedef const Operand &FValue3;
		typedef const Operand &FValue4;
		typedef const Operand &Tmp;

		void PS_2_0();
		void DCL(Dest);
		void DCL_2D(Dest);
		void DCL_CUBE(Dest);
		void DCL_VOLUME(Dest);
		void DEF(Dest, FValue1, FValue2, FValue3, FValue4);
		void ABS(Dst, Src);
		void ADD(Dst, Src0, Src1);
		void CMP(Dst, Src0, Src1, Src2);
		void CRS(Dst, Src0, Src1);
		void DP2ADD(Dst, Src0, Src1, Src2);
		void DP3(Dst, Src0, Src1);
		void DP4(Dst, Src0, Src1);
		void EXP(Dst, Src);
		void FRC(Dst, Src);
		void LOG(Dst, Src);
		void LRP(Dst, Src0, Src1, Src2);
		void M3X2(Dst, Src0, Src1);
		void M3X3(Dst, Src0, Src1);
		void M3X4(Dst, Src0, Src1);
		void M4X3(Dst, Src0, Src1);
		void M4X4(Dst, Src0, Src1);
		void MAD(Dst, Src0, Src1, Src2);
		void MAX(Dst, Src0, Src1);
		void MIN(Dst, Src0, Src1);
		void MOV(Dst, Src);
		void MUL(Dst, Src0, Src1);
		void NOP();
		void NRM(Dst, Src);
		void POW(Dst, Src0, Src1);
		void RCP(Dst, Src);
		void RSQ(Dst, Src);
		void SINCOS(Dst, Src0, Src1, Src2);
		void SUB(Dst, Src0, Src1);
		void TEXKILL(Src);
		void TEXLD(Dst, Src0, Src1);
		void TEXLDB(Dst, Src0, Src1);
		void TEXLDP(Dst, Src0, Src1);

		// Helper macro instructions
		void NEG(Tmp, Src);
		void SWIZZLE(Tmp, Src);
		void MASK(Dst, Tmp);
		void SAT(Dst, Tmp);

		void NEG_SWIZZLE(Tmp, Src);
		void SAT_MASK(Dst, Tmp);
		void SAT_MOV_X(Dst, Tmp);
		void SAT_MOV_XY(Dst, Tmp);
		void SAT_MOV_XYZ(Dst, Tmp);
		void SAT_MOV_XYZW(Dst, Tmp);

		// Overloaded to optimize
		void movaps(OperandXMMREG xmm1, OperandXMMREG xmm2);
		void movaps(OperandXMMREG xmm, OperandMEM128 m128);
		void movaps(OperandXMMREG xmm, OperandR_M128 r_m128);
		void movaps(OperandMEM128 m128, OperandXMMREG xmm);
		void movaps(OperandR_M128 r_m128, OperandXMMREG xmm);

		void movups(OperandXMMREG xmm1, OperandXMMREG xmm2);
		void movups(OperandXMMREG xmm, OperandMEM128 m128);
		void movups(OperandXMMREG xmm, OperandR_M128 r_m128);
		void movups(OperandMEM128 m128, OperandXMMREG xmm);
		void movups(OperandR_M128 r_m128, OperandXMMREG xmm);

		void movss(OperandXMMREG xmm1, OperandXMMREG xmm2);
		void movss(OperandXMMREG xmm, OperandMEM32 m32);
		void movss(OperandXMMREG xmm, OperandXMM32 r_m32);
		void movss(OperandMEM32 m32, OperandXMMREG xmm);
		void movss(OperandXMM32 r_m32, OperandXMMREG xmm);

		void movhlps(OperandXMMREG xmm1, OperandXMMREG xmm2);

		void movlhps(OperandXMMREG xmm1, OperandXMMREG xmm2);

		void shufps(OperandXMMREG xmm1, OperandXMMREG xmm2, unsigned char imm8);
		void shufps(OperandXMMREG xmm, OperandMEM128 m128, unsigned char imm8);
		void shufps(OperandXMMREG xmm, OperandR_M128 r_m128, unsigned char imm8);

		void pshufw(OperandMMREG mm1, OperandMMREG mm2, unsigned char imm8);
		void pshufw(OperandMMREG mm, OperandR_M64 r_m64, unsigned char imm8);
		void pshufw(OperandMMREG mm, OperandMEM64 mem64, unsigned char imm8);
	};
}

#endif   // swShader_PS20Assembler_hpp
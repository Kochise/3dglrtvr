#ifndef swShader_Operand_hpp
#define swShader_Operand_hpp

namespace swShader
{
	struct Operand
	{
		enum Type
		{
			VOID,

			COLOR_REGISTER,
			CONSTANT_FLOAT_REGISTER,
			INPUT_TEXTURE_COORDINATE_REGISTER,
			SAMPLER_REGISTER,
			TEMPORARY_REGISTER,

			OUTPUT_COLOR_REGISTER,
			OUTPUT_DEPTH_REGISTER,

			CONSTANT_FLOAT_LITERAL,

			// Used as internal temporary registers
			VIRTUAL_REGISTER
		};

		enum SourceModifier
		{
			NONE,
			NEGATE
		};

		struct Register
		{
			Register(Type type = Operand::VOID, int index = 0)
			{
				this->type = type;
				this->index = index;
			}

			Register(const Register &reg)
			{
				type = reg.type;
				index = reg.index;
			}

			Register(const Operand &op)
			{
				type = op.type;
				index = op.index;
			}

			operator==(const Register &reg) const
			{
				 return type == reg.type && index == reg.index;
			}

			Type type;

			union
			{
				int index;
				float value;
			};

			bool dcl;
		};

		enum Component
		{
			M = -1,   // Masked out

			X = 0,   // Copy x component
			Y = 1,   // Copy y component
			Z = 2,   // Copy z component
			W = 3,   // Copy w component
		};

		Operand(const Operand &op)
		{
			mod = op.mod;
			reg = op.reg;
			sel = op.sel;
		}

		Operand(const Register &reg)
		{
			mod = NONE;
			this->reg = reg;
			sel.x = X;
			sel.y = Y;
			sel.z = Z;
			sel.w = W;
		}

		Operand(SourceModifier mod = NONE, Register reg = VOID, Component a = X, Component b = Y, Component c = Z, Component d = W)
		{
			this->mod = mod;
			this->reg = reg;
			this->sel.x = a;
			this->sel.y = b;
			this->sel.z = c;
			this->sel.w = d;
		}

		Operand(Type type, int index = 0)
		{
			mod = NONE;
			this->type = type;
			this->index = index;
			sel.x = X;
			sel.y = Y;
			sel.z = Z;
			sel.w = W;
		}

		Operand &operator=(const Operand &op)
		{
			mod = op.mod;
			reg = op.reg;
			sel = op.sel;

			return *this;
		}

		operator==(const Register &reg) const
		{
			return this->reg.type == reg.type && this->reg.index == reg.index;
		}

		unsigned char swizzle() const
		{
			return (sel.x << 0) |
			       (sel.y << 2) |
			       (sel.z << 4) |
			       (sel.w << 6);
		}

		SourceModifier mod;

		union
		{
			struct
			{
				Register reg;
			};

			struct
			{
				Type type;

				union
				{
					int index;
					float value;
				};
			};
		};

		struct Select
		{
			Component x : 8;
			Component y : 8;
			Component z : 8;
			Component w : 8;

			operator bool() const
			{
				if(x == M && y == M && z == M && w == M)
				{
					return false;
				}

				if(x == X && y == Y && z == Z && w == W)
				{
					return false;
				}

				return true;
			}

			bool operator==(const Select &mask) const
			{
				return x == mask.x &&
				       y == mask.y &&
				       z == mask.z &&
				       w == mask.w;
			}

		} sel;
	};

	const Operand::Register voidRegister(Operand::VOID, 0);

	const Operand::Register v0Register(Operand::COLOR_REGISTER, 0);
	const Operand::Register v1Register(Operand::COLOR_REGISTER, 1);

	const Operand::Register c0Register(Operand::CONSTANT_FLOAT_REGISTER, 0);
	const Operand::Register c1Register(Operand::CONSTANT_FLOAT_REGISTER, 1);
	const Operand::Register c2Register(Operand::CONSTANT_FLOAT_REGISTER, 2);
	const Operand::Register c3Register(Operand::CONSTANT_FLOAT_REGISTER, 3);
	const Operand::Register c4Register(Operand::CONSTANT_FLOAT_REGISTER, 4);
	const Operand::Register c5Register(Operand::CONSTANT_FLOAT_REGISTER, 5);
	const Operand::Register c6Register(Operand::CONSTANT_FLOAT_REGISTER, 6);
	const Operand::Register c7Register(Operand::CONSTANT_FLOAT_REGISTER, 7);
	const Operand::Register c8Register(Operand::CONSTANT_FLOAT_REGISTER, 8);
	const Operand::Register c9Register(Operand::CONSTANT_FLOAT_REGISTER, 9);
	const Operand::Register c10Register(Operand::CONSTANT_FLOAT_REGISTER, 10);
	const Operand::Register c11Register(Operand::CONSTANT_FLOAT_REGISTER, 11);
	const Operand::Register c12Register(Operand::CONSTANT_FLOAT_REGISTER, 12);
	const Operand::Register c13Register(Operand::CONSTANT_FLOAT_REGISTER, 13);
	const Operand::Register c14Register(Operand::CONSTANT_FLOAT_REGISTER, 14);
	const Operand::Register c15Register(Operand::CONSTANT_FLOAT_REGISTER, 15);
	const Operand::Register c16Register(Operand::CONSTANT_FLOAT_REGISTER, 16);
	const Operand::Register c17Register(Operand::CONSTANT_FLOAT_REGISTER, 17);
	const Operand::Register c18Register(Operand::CONSTANT_FLOAT_REGISTER, 18);
	const Operand::Register c19Register(Operand::CONSTANT_FLOAT_REGISTER, 19);
	const Operand::Register c20Register(Operand::CONSTANT_FLOAT_REGISTER, 20);
	const Operand::Register c21Register(Operand::CONSTANT_FLOAT_REGISTER, 21);
	const Operand::Register c22Register(Operand::CONSTANT_FLOAT_REGISTER, 22);
	const Operand::Register c23Register(Operand::CONSTANT_FLOAT_REGISTER, 23);
	const Operand::Register c24Register(Operand::CONSTANT_FLOAT_REGISTER, 24);
	const Operand::Register c25Register(Operand::CONSTANT_FLOAT_REGISTER, 25);
	const Operand::Register c26Register(Operand::CONSTANT_FLOAT_REGISTER, 26);
	const Operand::Register c27Register(Operand::CONSTANT_FLOAT_REGISTER, 27);
	const Operand::Register c28Register(Operand::CONSTANT_FLOAT_REGISTER, 28);
	const Operand::Register c29Register(Operand::CONSTANT_FLOAT_REGISTER, 29);
	const Operand::Register c30Register(Operand::CONSTANT_FLOAT_REGISTER, 30);
	const Operand::Register c31Register(Operand::CONSTANT_FLOAT_REGISTER, 31);

	const Operand::Register t0Register(Operand::INPUT_TEXTURE_COORDINATE_REGISTER, 0);
	const Operand::Register t1Register(Operand::INPUT_TEXTURE_COORDINATE_REGISTER, 1);
	const Operand::Register t2Register(Operand::INPUT_TEXTURE_COORDINATE_REGISTER, 2);
	const Operand::Register t3Register(Operand::INPUT_TEXTURE_COORDINATE_REGISTER, 3);
	const Operand::Register t4Register(Operand::INPUT_TEXTURE_COORDINATE_REGISTER, 4);
	const Operand::Register t5Register(Operand::INPUT_TEXTURE_COORDINATE_REGISTER, 5);
	const Operand::Register t6Register(Operand::INPUT_TEXTURE_COORDINATE_REGISTER, 6);
	const Operand::Register t7Register(Operand::INPUT_TEXTURE_COORDINATE_REGISTER, 7);

	const Operand::Register s0Register(Operand::SAMPLER_REGISTER, 0);
	const Operand::Register s1Register(Operand::SAMPLER_REGISTER, 1);
	const Operand::Register s2Register(Operand::SAMPLER_REGISTER, 2);
	const Operand::Register s3Register(Operand::SAMPLER_REGISTER, 3);
	const Operand::Register s4Register(Operand::SAMPLER_REGISTER, 4);
	const Operand::Register s5Register(Operand::SAMPLER_REGISTER, 5);
	const Operand::Register s6Register(Operand::SAMPLER_REGISTER, 6);
	const Operand::Register s7Register(Operand::SAMPLER_REGISTER, 7);
	const Operand::Register s8Register(Operand::SAMPLER_REGISTER, 8);
	const Operand::Register s9Register(Operand::SAMPLER_REGISTER, 9);
	const Operand::Register s10Register(Operand::SAMPLER_REGISTER, 10);
	const Operand::Register s11Register(Operand::SAMPLER_REGISTER, 11);
	const Operand::Register s12Register(Operand::SAMPLER_REGISTER, 12);
	const Operand::Register s13Register(Operand::SAMPLER_REGISTER, 13);
	const Operand::Register s14Register(Operand::SAMPLER_REGISTER, 14);
	const Operand::Register s15Register(Operand::SAMPLER_REGISTER, 15);

	const Operand::Register r0Register(Operand::TEMPORARY_REGISTER, 0);
	const Operand::Register r1Register(Operand::TEMPORARY_REGISTER, 1);
	const Operand::Register r2Register(Operand::TEMPORARY_REGISTER, 2);
	const Operand::Register r3Register(Operand::TEMPORARY_REGISTER, 3);
	const Operand::Register r4Register(Operand::TEMPORARY_REGISTER, 4);
	const Operand::Register r5Register(Operand::TEMPORARY_REGISTER, 5);
	const Operand::Register r6Register(Operand::TEMPORARY_REGISTER, 6);
	const Operand::Register r7Register(Operand::TEMPORARY_REGISTER, 7);
	const Operand::Register r8Register(Operand::TEMPORARY_REGISTER, 8);
	const Operand::Register r9Register(Operand::TEMPORARY_REGISTER, 9);
	const Operand::Register r10Register(Operand::TEMPORARY_REGISTER, 10);
	const Operand::Register r11Register(Operand::TEMPORARY_REGISTER, 11);

	const Operand::Register oC0Register(Operand::OUTPUT_COLOR_REGISTER, 0);
	const Operand::Register oC1Register(Operand::OUTPUT_COLOR_REGISTER, 1);
	const Operand::Register oC2Register(Operand::OUTPUT_COLOR_REGISTER, 2);
	const Operand::Register oC3Register(Operand::OUTPUT_COLOR_REGISTER, 3);

	const Operand::Register oDepthRegister(Operand::OUTPUT_DEPTH_REGISTER);

	const Operand voidOperand;   // Default constructor creates void

	const Operand::Select xMask = {Operand::X, Operand::M, Operand::M, Operand::M};
	const Operand::Select yMask = {Operand::M, Operand::Y, Operand::M, Operand::M};
	const Operand::Select zMask = {Operand::M, Operand::M, Operand::Z, Operand::M};
	const Operand::Select wMask = {Operand::M, Operand::M, Operand::M, Operand::W};
	const Operand::Select xyMask = {Operand::X, Operand::Y, Operand::M, Operand::M};
	const Operand::Select xzMask = {Operand::X, Operand::M, Operand::Z, Operand::M};
	const Operand::Select xwMask = {Operand::X, Operand::M, Operand::M, Operand::W};
	const Operand::Select yzMask = {Operand::M, Operand::Y, Operand::Z, Operand::M};
	const Operand::Select ywMask = {Operand::M, Operand::Y, Operand::M, Operand::W};
	const Operand::Select zwMask = {Operand::M, Operand::M, Operand::Z, Operand::W};
	const Operand::Select xyzMask = {Operand::X, Operand::Y, Operand::Z, Operand::M};
	const Operand::Select xywMask = {Operand::X, Operand::Y, Operand::M, Operand::W};
	const Operand::Select xzwMask = {Operand::X, Operand::M, Operand::Z, Operand::W};
	const Operand::Select yzwMask = {Operand::M, Operand::Y, Operand::Z, Operand::W};
	const Operand::Select xyzwMask = {Operand::X, Operand::Y, Operand::Z, Operand::W};
}

#endif
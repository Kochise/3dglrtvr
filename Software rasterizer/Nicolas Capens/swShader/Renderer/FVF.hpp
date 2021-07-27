#ifndef FVF_hpp
#define FVF_hpp

#include "MetaMacro.hpp"
#include "Point.hpp"
#include "Vector.hpp"
#include "Color.hpp"
#include "Types.hpp"

namespace swShader
{
	enum
	{
		FVF_POSITION		= 0x0000,
		FVF_POINT			= 0x0000,
		FVF_RHW				= 0x0001,
		FVF_NORMAL			= 0x0002,
		FVF_COLOR			= 0x0004,
		FVF_DIFFUSE			= 0x0004,
		FVF_LIGHT			= 0x0008,
		FVF_SPECULAR		= 0x0008,

		FVF_TEX_MASK		= 0x0F00,
		FVF_TEX_SHIFT		= 8,
		FVF_TEX				= 0x0100,
		FVF_TEX1			= 0x0100,
		FVF_TEX2			= 0x0200,
		FVF_TEX3			= 0x0300,
		FVF_TEX4			= 0x0400,
		FVF_TEX5			= 0x0500,
		FVF_TEX6			= 0x0600,
		FVF_TEX7			= 0x0700,
		FVF_TEX8			= 0x0800,

		FVF_VERTEX			= FVF_NORMAL | FVF_TEX,
		FVF_LVERTEX			= FVF_DIFFUSE | FVF_SPECULAR | FVF_TEX,
		FVF_LTVERTEX		= FVF_RHW | FVF_DIFFUSE | FVF_SPECULAR | FVF_TEX,
		FVF_XVERTEX			= FVF_RHW | FVF_DIFFUSE | FVF_SPECULAR | FVF_TEX8
	};

	struct UV
	{
		float u;
		float v;
	};

	ALIGN(16) struct UVW
	{
		UVW()
		{
		}

		UVW(const UV &uv)
		{
			u = uv.u;
			v = uv.v;
		}

		float u;
		float v;
		float w;
		float m;
	};

	template<unsigned int flags>
	class FVFGenerator
	{
		struct Position
		{
			union
			{
				struct
				{
					float x;
					float y;
					float z;
				};

				struct
				{
					float Px;
					float Py;
					float Pz;
				};

				struct
				{
					Point P;
				};
			};
		};

		struct RHW
		{
			union
			{
				float rhw;
				float w;
				float Pw;
			};
		};

		struct Normal
		{
			union
			{
				struct
				{
					float Nx;
					float Ny;
					float Nz;
				};

				struct
				{
					Vector N;
				};
			};
		};

		struct Diffuse 
		{
			union
			{
				struct
				{
					float Cb;
					float Cg;
					float Cr;
					float Ca;
				};

				struct
				{
					Color<float> C;
				};
			};
		};

		struct Specular
		{
			union
			{
				struct
				{
					float Lb;
					float Lg;
					float Lr;
					float La;
				};

				struct
				{
					Color<float> L;
				};
			};
		};

		struct Tex0
		{
			union
			{
				struct
				{
					float u;
					float v;
				};

				UV T[1];

				struct
				{
					float u0;
					float v0;
				};
			};
		};

		struct Tex1
		{
			union
			{
				float s;
				float u1;
			};

			union
			{
				float t;
				float v1;
			};
		};

		struct Tex2
		{
			float u2;
			float v2;
		};

		struct Tex3
		{
			float u3;
			float v3;
		};

		struct Tex4
		{
			float u4;
			float v4;
		};

		struct Tex5
		{
			float u5;
			float v5;
		};

		struct Tex6
		{
			float u6;
			float v6;
		};

		struct Tex7
		{
			float u7;
			float v7;
		};

	public:
		enum
		{
			hasRHW			= !!(flags & FVF_RHW),
			hasNormal		= !!(flags & FVF_NORMAL),
			hasDiffuse		= !!(flags & FVF_DIFFUSE),
			hasSpecular		= !!(flags &FVF_SPECULAR),
			hasTexCoords	= !!(flags & FVF_TEX_MASK),
			numTexCoords	= (flags & FVF_TEX_MASK) >> FVF_TEX_SHIFT
		};

	private:
		META_ASSERT(numTexCoords <= 8);

		typedef META_CONDITIONAL_INHERIT(hasRHW, Position, RHW) V1;
		typedef META_CONDITIONAL_INHERIT(hasNormal, V1, Normal) V2;
		typedef META_CONDITIONAL_INHERIT(hasDiffuse, V2, Diffuse) V3;
		typedef META_CONDITIONAL_INHERIT(hasSpecular, V3, Specular) V4;
		typedef META_CONDITIONAL_INHERIT((numTexCoords > 0), V4, Tex0) V5;
		typedef META_CONDITIONAL_INHERIT((numTexCoords > 1), V5, Tex1) V6;
		typedef META_CONDITIONAL_INHERIT((numTexCoords > 2), V6, Tex2) V7;
		typedef META_CONDITIONAL_INHERIT((numTexCoords > 3), V7, Tex3) V8;
		typedef META_CONDITIONAL_INHERIT((numTexCoords > 4), V8, Tex4) V9;
		typedef META_CONDITIONAL_INHERIT((numTexCoords > 5), V9, Tex5) V10;
		typedef META_CONDITIONAL_INHERIT((numTexCoords > 6), V10, Tex6) V11;
		typedef META_CONDITIONAL_INHERIT((numTexCoords > 7), V11, Tex7) V12;

	public:	
		enum
		{
			offRHW		= sizeof(Point),
			offNormal	= sizeof(V1),
			offDiffuse	= sizeof(V2),
			offSpecular	= sizeof(V3),
			offTex0		= sizeof(V4),
			offTex1		= sizeof(V5),
			offTex2		= sizeof(V6),
			offTex3		= sizeof(V7),
			offTex4		= sizeof(V8),
			offTex5		= sizeof(V9),
			offTex6		= sizeof(V10),
			offTex7		= sizeof(V11)
		};

		typedef ALIGN(16) V12 Res;
	};

	template<unsigned int flags>
	struct FVF : public FVFGenerator<flags>::Res, public FVFGenerator<flags>
	{
	};
}

#endif   // FVF_hpp
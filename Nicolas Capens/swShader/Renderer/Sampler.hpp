#ifndef Sampler_hpp
#define Sampler_hpp

#include "Color.hpp"

namespace swShader
{
	class Texture;
	class Rasterizer;
	class PS20Assembler;

	class Sampler
	{
		friend Rasterizer;
		friend PS20Assembler;

	public:
		enum FilterType
		{
			FILTER_POINT,
			FILTER_LINEAR,
		};

		enum AddressingMode
		{
			ADDRESSING_WRAP,
			ADDRESSING_CLAMP,
			ADDRESSING_MIRROR
		};

		Sampler();

		~Sampler();

		void setTextureMap(Texture *texture);
		void releaseTexture();

		bool setTextureFilter(FilterType textureFilter);
		bool setAddressingMode(AddressingMode addressingMode);

		int status() const;

	private:
		Texture *texture;

	//	Color<float> borderColor;
		int textureFilter;
		int addressingMode;
	//	int mipmapFilter;
	//	float mipmapBias;
	//	float gamma;
	};
}

#endif   // Sampler_hpp
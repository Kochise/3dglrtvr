#include "Sampler.hpp"

#include "Error.hpp"
#include "Texture.hpp"

namespace swShader
{
	Sampler::Sampler()
	{
		texture = 0;

	//	borderColor = Color<float>::BLACK;
		textureFilter = FILTER_LINEAR;
		addressingMode = ADDRESSING_WRAP;
	//	mipmapFilter = FILTER_POINT;
	//	mipmapBias = 0;
	//	gamma = 2.2f;
	}

	Sampler::~Sampler()
	{
		if(texture)
		{
			texture->release();
			texture = 0;
		}
	}

	void Sampler::setTextureMap(Texture *texture)
	{
		if(!texture) throw Error("Invalid texture");

		if(this->texture) this->texture->release();

		this->texture = texture;
		texture->aquire();
	}

	void Sampler::releaseTexture()
	{
		if(texture)
		{
			texture->release();
			texture = 0;
		}
	}

	bool Sampler::setTextureFilter(FilterType textureFilter)
	{
		bool update = this->textureFilter != textureFilter;
		this->textureFilter = textureFilter;
		return update;
	}

	bool Sampler::setAddressingMode(AddressingMode addressingMode)
	{
		bool update = this->addressingMode != addressingMode;
		this->addressingMode = addressingMode;
		return update;
	}

	int Sampler::status() const
	{
		return textureFilter  << 0 |
		       addressingMode << 4;
	}
}
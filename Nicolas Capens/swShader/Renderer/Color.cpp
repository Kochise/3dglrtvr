#include "Color.hpp"

namespace swShader
{
	const Color<float> Color<float>::WHITE(1.0f, 1.0f, 1.0f);
	const Color<float> Color<float>::BLACK(0.0f, 0.0f, 0.0f);
	const Color<float> Color<float>::GRAY(0.5f, 0.5f, 0.5f);
	const Color<float> Color<float>::BLUE(0.0f, 0.0f, 1.0f);
	const Color<float> Color<float>::CYAN(0.0f, 1.0f, 1.0f);
	const Color<float> Color<float>::GREEN(0.0f, 1.0f, 0.0f);
	const Color<float> Color<float>::YELLOW(1.0f, 1.0f, 0.0f);
	const Color<float> Color<float>::RED(1.0f, 0.0f, 0.0f);
	const Color<float> Color<float>::MAGENTA(1.0f, 0.0f, 1.0f);

	float Color<byte>::gamma = 0;
	float Color<float>::gamma = 0;

	byte Color<byte>::averageLUT[256][256];
	byte Color<byte>::gammaLUT[256];
}
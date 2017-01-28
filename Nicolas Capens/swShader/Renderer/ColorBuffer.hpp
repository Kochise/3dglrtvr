#ifndef ColorBuffer_hpp
#define ColorBuffer_hpp

#include "Lockable.hpp"

namespace swShader
{
	class ColorBuffer : public Lockable
	{
	public:
		virtual void *getColorBuffer() = 0;
		virtual int getWidth() = 0;
		virtual int getHeight() = 0;
		virtual int getStride();
		virtual int getBitDepth() = 0;
	};
}

#endif   // ColorBuffer_hpp
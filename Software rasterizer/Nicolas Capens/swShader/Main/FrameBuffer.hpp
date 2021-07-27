#ifndef	FrameBuffer_hpp
#define	FrameBuffer_hpp

#include "RenderTarget.hpp"
#include "ColorBuffer.hpp"
#include "Color.hpp"

#include <ddraw.h>

namespace swShader
{
	class FrameBuffer : public RenderTarget
	{
	public:
		FrameBuffer();

		virtual ~FrameBuffer();

		void initFullscreen(HWND windowHandle, int width, int height);
		void initWindowed(HWND windowHandle, int width, int height);
		void updateBounds();

		void lock();
		void unlock();

		void *getColorBuffer();
		void *getDepthBuffer();
		int getWidth();
		int getHeight();
		int getStride();
		int getBitDepth();

		void clear(Color<byte> color = 0);
		void flip();

		void releaseAll();

	protected:
		IDirectDraw7 *directDraw;
		IDirectDrawSurface7	*frontBuffer;
		IDirectDrawSurface7	*backBuffer;

		bool locked;

		void *colorBuffer;
		void *depthBuffer;
		int width;
		int height;
		int bitDepth;
		int pitch;
		int stride;

		HWND windowHandle;
		RECT bounds;
		bool windowed;
	};
}

#endif	 //	FrameBuffer_hpp
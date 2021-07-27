#ifndef RenderTarget_hpp
#define RenderTarget_hpp

namespace swShader
{
	class RenderTarget
	{
	public:
		virtual void *getColorBuffer() = 0;
		virtual void *getDepthBuffer() = 0;
		virtual int getWidth() = 0;
		virtual int getHeight() = 0;
		virtual int getStride();
		virtual int getBitDepth() = 0;
	};
}

#endif   // RenderTarget_hpp
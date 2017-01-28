#ifndef Application_hpp
#define Application_hpp

#include "Window.hpp"

namespace swShader
{
	class Renderer;
	class Texture;
	class Model3DS;
	class FrameBuffer;

	class Application : public Window
	{
	public:
		Application(HINSTANCE instance);

		~Application();

	protected:
		void init();
		void destroy();

		void eventDraw();
		void eventKeyDown(int key);
		
		FrameBuffer *getFrameBuffer() const;
		void renderFrame();

	private:
		FrameBuffer *frameBuffer;
		Renderer *renderer;

		Texture *texture[2];
		Model3DS *model;

		int frame;
		float elapsedTime;
		double totalTime;
	};
}

#endif   // Application_hpp
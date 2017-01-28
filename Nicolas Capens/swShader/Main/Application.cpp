#include "Application.hpp"

#include "FrameBuffer.hpp"
#include "Error.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "VertexBuffer.hpp"
#include "XVertex.hpp"
#include "Color.hpp"
#include "Matrix.hpp"
#include "Sampler.hpp"
#include "Timer.hpp"
#include "Model3DS.hpp"

namespace swShader
{
	Application::Application(HINSTANCE instance) : renderer(0), Window(instance, "swShader Demo", 640, 480)
	{
		frameBuffer = 0;
		init();
	}

	Application::~Application()
	{
		destroy();
	}

	void Application::init()
	{
		frame = 0;

		elapsedTime = 0;
		totalTime = 0;

		delete frameBuffer;
		frameBuffer = new FrameBuffer();

		if(style & FULLSCREEN)
		{
			hideCursor();
			frameBuffer->initFullscreen(handle, width, height);
		}
		else
		{
			showCursor();
			frameBuffer->initWindowed(handle, width, height);
		}

		renderer = new Renderer(frameBuffer);

		renderer->setShader("Shader/Shader.txt");

		texture[0] = new Texture("Crate.jpg");
		texture[1] = new Texture("Lightning.jpg");

		renderer->setTextureMap(0, texture[0]);
		renderer->setTextureMap(1, texture[1]);

		frameBuffer->clear();
		frameBuffer->flip();

		model = new Model3DS("Teapot.3ds");
	}

	void Application::destroy()
	{
		delete frameBuffer;
		frameBuffer = 0;

		delete renderer;
		renderer = 0;

		delete texture[0];
		delete texture[1];
		texture[0] = 0;
		texture[1] = 0;

		delete model;
		model = 0;
	}

	void Application::eventDraw()
	{
		static double previousTime = Timer::seconds();

		try
		{
			frameBuffer->lock();

			renderFrame();

			frame++;

			const double currentTime = Timer::seconds();
			elapsedTime = (float)(currentTime - previousTime);
			totalTime += elapsedTime;
			previousTime = currentTime;

			frameBuffer->updateBounds();
			frameBuffer->flip();
		}
		catch(const Error &error)
		{
			if(style & FULLSCREEN)
			{
				toggleFullscreen();
			}

			MessageBox(handle, error.getString(), "ERROR", MB_OK);
		}
	}

	void Application::eventKeyDown(int key)
	{
		if(key == VK_ESCAPE)
		{
			terminate();
		}
	}

	FrameBuffer *Application::getFrameBuffer() const
	{
		return frameBuffer;
	}

	void Application::renderFrame()
	{
		if(!renderer || !frameBuffer) throw Error("Initialization error");

		frameBuffer->clear();

		float pulse[4];
		pulse[0] = pulse[1] = pulse[2] = pulse[3] = sin((float)totalTime);

		renderer->setPixelShaderConstant(0, pulse);
		renderer->setModelTransform(Matrix::eulerRotate(0.2f * (float)totalTime,
			                                            0.4f * (float)totalTime,
														0.3f * (float)totalTime));
		renderer->setViewTransform(Matrix::translate(0, 125, 0));
	
		renderer->drawPrimitive(model, model);
	}
}
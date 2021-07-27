#include "Viewport.hpp"

#include "RenderTarget.hpp"
#include "Error.hpp"
#include "Math.hpp"

namespace swShader
{
	Viewport::Viewport(RenderTarget *renderTarget)
	{
		if(!renderTarget) throw Error("Could not initialize viewport");

		this->renderTarget = renderTarget;
		
		left = 0;
		top = 0;
		width = (float)renderTarget->getWidth();
		height = (float)renderTarget->getHeight();
	}

	Viewport::~Viewport()
	{
		if(!renderTarget) throw INTERNAL_ERROR;
	}

	void Viewport::setLeft(float l)
	{
		clamp(l, 0.0f, (float)renderTarget->getWidth());

		left = l;
	}

	void Viewport::setTop(float t)
	{
		clamp(t, 0.0f, (float)renderTarget->getHeight());

		top = t;
	}

	void Viewport::setWidth(float w)
	{
		clamp(w, 0.0f, (float)renderTarget->getWidth() - left);

		width = w;
	}

	void Viewport::setHeight(float h)
	{
		clamp(h, 0.0f, (float)renderTarget->getHeight() - top);

		height = h;
	}

	float Viewport::getLeft() const
	{
		return left;
	}

	float Viewport::getTop() const
	{
		return top;
	}

	float Viewport::getWidth() const
	{
		return width;
	}

	float Viewport::getHeight() const
	{
		return height;
	}
}
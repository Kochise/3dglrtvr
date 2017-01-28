#ifndef Viewport_hpp
#define Viewport_hpp

namespace swShader
{
	class RenderTarget;

	class Viewport
	{
	public:
		Viewport(RenderTarget *renderTarget);

		~Viewport();

		void setLeft(float l);
		void setTop(float t);
		void setWidth(float w);
		void setHeight(float h);

		float getLeft() const;
		float getTop() const;
		float getWidth() const;
		float getHeight() const;

	private:
		RenderTarget *renderTarget;

		float left;     // Leftmost pixel column
		float top;      // Highest pixel row
		float width;    // Width in pixels
		float height;   // Height in pixels
	};
}

#endif   // Viewport_hpp
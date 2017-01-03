
#include "renderer.h"
#include "exceptions.h"
#include "logconsole.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::ComputeVectors(const View& view, const Image& image)
{
	Vector3d n (view.point, view.target);
    n.normalize();

	v = view.upperdirection.normalized();
    v -= n * (v | n);
	v.normalize();

	h = (n ^ v);

	if (!(view.width > 0))
		throw InvalidParameterException ("View : Invalid view width");
	if (!(view.height > 0))
		throw InvalidParameterException ("View : Invalid view height");

	v = v * (view.width / image.width());
    h = h * (view.height/ image.height());
}

void Renderer::render(const Scene& scene, const View& view,
                      const RayTracer& tracer, Image& image)
{
	ComputeVectors(view,image);
	Vector3d n (view.point, view.target);

	Utils::log.msg("Rendering Started ...");

	double calibrate_scroll = 100.0 / (image.height() * image.width());
	int progress = 0;
	int demiwidth = image.width() / 2;
	int demiheight = image.height() / 2;
	
	for (int y = 0; y < image.height(); y++) 
		for (int x = 0; x < image.width(); x++)
		{
			int currentprogress = int((float) (y * image.width() + x + 1) * calibrate_scroll);
			if (currentprogress > progress)
			{
				Utils::log.msg("Done " + Utils::value2string(currentprogress) + "%");
				progress++;
			}

			Vector3d raydirection = n + h * 1.0 * (x - demiwidth) + v * 1.0 * (demiheight - y);
			Ray currentray ( view.point, raydirection );
			Color c = renderPixel(scene, tracer, currentray);
			image.setPixelAt(x,y,c);
		}
	Utils::log.msg("Rendering finished ");
}

Vector3d Renderer::getHorizontalVector() const
{
    return h;
}

Vector3d Renderer::getVerticalVector() const
{
    return v;
}


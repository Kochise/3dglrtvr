
#include "lightraytracer.h"
#include "intersections.h"
#include "point3d.h"
#include "vector3d.h"

LightRayTracer::LightRayTracer(const Color& _ambientcolor, const Color& _ambientlight)
: SimplestRayTracer(_ambientcolor), ambientlight(_ambientlight)
{
}

LightRayTracer::~LightRayTracer()
{
}

std::string LightRayTracer::toString() const
{
    return "LightRayTracer { "
        + Utils::value2string("AmbientColor", ambientcolor) + ' '
		+ Utils::value2string("AmbientLight", ambientlight) + " }";
}

Color LightRayTracer::computeColor (const Ray& ray, const Scene& scene, const IntersectionData& id, const SurfaceData& sd, const Color& lc) const
{
    // Compute the sum of all incoming light
	Color lightcolor = ambientlight;
	for (int i = 0; i < scene.getLightNumber(); i++)
	{
	    Light * light = scene.getLight(i);
	    Point3d globalpoint = id.object->convertPoint3dToGlobal(id.localpoint);
		  // Computes the percent of light touching the surface, using the normal and light specific parameters
	    lightcolor += light->color * light->getVisibilityFromPoint(globalpoint, sd.globalnormal, &scene);
	}
    // Apply incoming light to local color
	return lightcolor * lc;
}


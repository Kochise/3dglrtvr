
#include "simplestraytracer.h"
#include "intersections.h"
#include "point3d.h"
#include "vector3d.h"

SimplestRayTracer::SimplestRayTracer(const Color& _ambientcolor)
: ambientcolor(_ambientcolor)
{
}

SimplestRayTracer::~SimplestRayTracer()
{
}

std::string SimplestRayTracer::toString() const
{
    return "SimplestRayTracer { "
        + Utils::value2string("AmbientColor", ambientcolor) + " }";
}

Color SimplestRayTracer::trace(const Ray& ray, const Scene& scene) const
{
	// Computes all intersection from current ray with objects
    Intersections intersections(&ray, &scene);

	// Find the first object visible (from ray point of view)
	const IntersectionData * const pid = intersections.getFirstVisible();

	// If no object has been reached, then use ambient color
	if (pid == NULL)
		return ambientcolor;
	
    // Compute the internals of the object
    SurfaceData sfd = pid->object->getSurfaceData(pid->localpoint);

	// Get the material from the object
	Material * material = scene.getMaterial(pid->object->materialname);
	Color localcolor = material->getColorAt(sfd.u,sfd.v);

	// Combine everything to get final color
	return computeColor(ray,scene,*pid,sfd,localcolor);
}

Color SimplestRayTracer::computeColor (const Ray& ray, const Scene& scene,
    const IntersectionData& id, const SurfaceData& sd, const Color& lc) const
{
    return lc;
}


#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "ray.h"
#include "scene.h"

class RayTracer : public Printable
{
    public:
        RayTracer();
        virtual ~RayTracer();
        
        virtual Color trace(const Ray& ray, const Scene& scene) const = 0;
        virtual Color computeColor (const Ray& ray, const Scene& scene, const IntersectionData& id, const SurfaceData& sd, const Color& lc) const = 0;
};

#endif

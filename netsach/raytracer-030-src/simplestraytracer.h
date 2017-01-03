#ifndef SIMPLESTRAYTRACER_H
#define SIMPLESTRAYTRACER_H

#include "raytracer.h"

class SimplestRayTracer : public RayTracer
{
    public:
    
        Color ambientcolor;
    
        SimplestRayTracer(const Color& _ambientcolor);
        virtual ~SimplestRayTracer();
        
        virtual Color trace(const Ray& ray, const Scene& scene) const;

        virtual std::string toString() const;
        
        virtual Color computeColor (const Ray& ray, const Scene& scene, const IntersectionData& id, const SurfaceData& sd, const Color& lc) const;
};

#endif

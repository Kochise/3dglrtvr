#ifndef LIGHTRAYTRACER_H
#define LIGHTRAYTRACER_H

#include "simplestraytracer.h"

class LightRayTracer : public SimplestRayTracer
{
    public:
        Color ambientlight;
    
        LightRayTracer(const Color& _ambientcolor, const Color& _ambientlight);
        virtual ~LightRayTracer();
        
        virtual std::string toString() const;
        
        virtual Color computeColor (const Ray& ray, const Scene& scene, const IntersectionData& id, const SurfaceData& sd, const Color& lc) const;

};

#endif
